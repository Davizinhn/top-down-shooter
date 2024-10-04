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

Textures::Textures() {
}

void Textures::LoadAll() {
    vector<string> folders = {"resources/items/", "resources/player/", "resources/scenary/"};
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