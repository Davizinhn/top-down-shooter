#include "global.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <regex>

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

vector<Projectile> Textures::GetProjectiles() {
    return projectiles;
}