#include "global.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <regex>
#include <raymath.h>
#include "player.hpp"

vector<string> getDirectoryFiles(const string& directoryPath, const vector<string>& extensions) {
    vector<string> fileList;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directoryPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string fileName = ent->d_name;

            if (fileName == "." || fileName == "..") {
                continue;
            }

            const char* fileExtension = strrchr(ent->d_name, '.');
            if (fileExtension != NULL) {
                for (const auto& ext : extensions) {
                    if (strcmp(fileExtension, ext.c_str()) == 0) {
                        fileList.push_back(directoryPath + fileName);
                        break;
                    }
                }
            }
        }
        closedir(dir);
    } else {
        perror("Directory not found.");
    }

    return fileList;
}

// textures uwu

Textures::Textures() {
}

void Textures::LoadAll() {
    vector<string> folders = {"resources/items/", "resources/player/", "resources/scenary/", "resources/"};
    vector<string> fileList;
    for(int j = 0; j < (int)folders.size(); j++) {
        fileList = getDirectoryFiles(folders[j], {".png"});
        for(int i = 0; i<(int)fileList.size(); i++) {
            vector<string> splittedString = SplitString(fileList[i], "/");
            list[(regex_replace(splittedString[(int)splittedString.size()-1], regex("\\.png"), ""))] = LoadTextureFromImage(LoadImage(fileList[i].c_str()));
        }
    }
}

void Textures::UnloadAll() {
    for (const auto& kv : list) {
        UnloadTexture(kv.second);
    }
}

Texture2D Textures::get(string name) {
    return list[name];
}

vector<string> Textures::SplitString(const string& str, const string& regex_str) {
    regex regexz(regex_str);
    vector<string> list(
        sregex_token_iterator(str.begin(), str.end(), regexz, -1),
        sregex_token_iterator()
    );
    return list;
}

void Textures::SpawnProjectile(Projectile newProjectile) {
    projectiles.push_back(newProjectile);
}

void Textures::SpawnEnemy(Enemy newEnemy) {
    enemies.push_back(newEnemy);
}

bool Textures::CheckCollisionPolygons(const vector<Vector2>& poly1, const vector<Vector2>& poly2) {
    auto checkOverlapOnAxis = [](const Vector2& axis, const vector<Vector2>& poly1, const vector<Vector2>& poly2) -> bool {
        float min1 = INFINITY, max1 = -INFINITY, min2 = INFINITY, max2 = -INFINITY;

        for (const auto& point : poly1) {
            float projection = Vector2DotProduct(point, axis);
            min1 = fminf(min1, projection);
            max1 = fmaxf(max1, projection);
        }

        for (const auto& point : poly2) {
            float projection = Vector2DotProduct(point, axis);
            min2 = fminf(min2, projection);
            max2 = fmaxf(max2, projection);
        }

        return max1 >= min2 && max2 >= min1;
    };

    vector<Vector2> axes;

    for (size_t i = 0; i < poly1.size(); i++) {
        Vector2 edge = Vector2Subtract(poly1[(i + 1) % poly1.size()], poly1[i]);
        axes.push_back(Vector2Normalize({-edge.y, edge.x}));
    }

    for (size_t i = 0; i < poly2.size(); i++) {
        Vector2 edge = Vector2Subtract(poly2[(i + 1) % poly2.size()], poly2[i]);
        axes.push_back(Vector2Normalize({-edge.y, edge.x}));
    }

    for (const auto& axis : axes) {
        if (!checkOverlapOnAxis(axis, poly1, poly2)) {
            return false;
        }
    }

    return true;
}

float Textures::RandomFloat(float min, float max) {
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
}

Vector2 Textures::RandomVector2(Vector2 min, Vector2 max) {
    return Vector2({RandomFloat(min.x, max.x), RandomFloat(min.y, max.y)});
}

// sounds :3

Sounds::Sounds() {
}

void Sounds::LoadAll() {
    vector<string> folders = {"resources/sounds/"};
    vector<string> fileList;
    for(int j = 0; j < (int)folders.size(); j++) {
        fileList = getDirectoryFiles(folders[j], {".wav"});
        for(int i = 0; i<(int)fileList.size(); i++) {
            vector<string> splittedString = Textures::shared_instance().SplitString(fileList[i], "/");
            list[(regex_replace(splittedString[(int)splittedString.size()-1], regex("\\.wav"), ""))] = LoadSound(fileList[i].c_str());
        }
    }
    vector<string> foldersMusic = {"resources/sounds/music/"};
    vector<string> fileListMusic;
    for(int j = 0; j < (int)foldersMusic.size(); j++) {
        fileListMusic = getDirectoryFiles(foldersMusic[j], {".wav"});
        for(int i = 0; i<(int)fileListMusic.size(); i++) {
            vector<string> splittedString = Textures::shared_instance().SplitString(fileListMusic[i], "/");
            musicList[(regex_replace(splittedString[(int)splittedString.size()-1], regex("\\.wav"), ""))] = LoadMusicStream(fileListMusic[i].c_str());
        }
    }
}

void Sounds::UnloadAll() {
    for (const auto& kv : list) {
        UnloadSound(kv.second);
    }
    for (const auto& kv : musicList) {
        UnloadMusicStream(kv.second);
    }
}

Sound Sounds::get(string name) {
    return list[name];
}

Music Sounds::getMusic(string name) {
    return musicList[name];
}

// GAME MANAGER owo

GameManager::GameManager() {
}

void GameManager::Update() {
    if(Textures::shared_instance().enemies.size() == 0 && timeBetweenLevels > 0) {
        timeBetweenLevels--;
        if(timeBetweenLevels == 0) canProceed = true;
    }

    if(timeBetweenLevels == 0 && canProceed) {
        NextLevel();
    }
}

int GameManager::GetCurrentLevel() {
    return curLevel;
}

void GameManager::NextLevel() {
    canProceed = false;
    curLevel++;
    levelEnemy += GetRandomValue(minIncrementValue, maxIncrementValue);

    float minRadius = 1200.0f;
    float maxRadius = 2000.0f;
    
    for(int i = 0; i < levelEnemy; i++) {
        float angle = Textures::shared_instance().RandomFloat(0, 2 * PI);
        float radius = Textures::shared_instance().RandomFloat(minRadius, maxRadius);
        
        Vector2 spawnPos = {
            playerPos.x + radius * cosf(angle),
            playerPos.y + radius * sinf(angle)
        };
        
        Textures::shared_instance().SpawnEnemy({spawnPos});
    }
    
    timeBetweenLevels = defaultTime;
    PlaySound(Sounds::shared_instance().get("nextLevel"));
}

void GameManager::Reset() {
    Textures::shared_instance().enemies = vector<Enemy>();
    Textures::shared_instance().projectiles = vector<Projectile>();
    curLevel = 0;
    timeBetweenLevels = 200;
    defaultTime = 200;
    minIncrementValue = 2;
    maxIncrementValue = 6;
    levelEnemy = 5;
    canProceed = false;
}

float GameManager::GetMapScale() {
    return mapScale;
}


Utils::Utils() {
}

Vector2 Utils::Vector2Lerp(Vector2 v1, Vector2 v2, float t) {
    Vector2 result = {0, 0};
    result.x = Lerp(v1.x, v2.x, t);
    result.y = Lerp(v1.y, v2.y, t);

    return result;
}

Color Utils::RandomDesaturatedColor() {
    float h = GetRandomValue(0, 360);
    float s = GetRandomValue(10, 20) / 100.0f;
    float v = GetRandomValue(85, 95) / 100.0f;

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h < 60) { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    return {(unsigned char)((r + m) * 255), (unsigned char)((g + m) * 255), (unsigned char)((b + m) * 255), 255};
}
