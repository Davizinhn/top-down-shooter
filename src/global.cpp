#include "global.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <regex>

std::vector<std::string> getDirectoryFiles(const std::string& directoryPath, const std::vector<std::string>& extensions) {
    std::vector<std::string> fileList;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directoryPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;

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
        perror("Erro ao abrir o diretório");
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
            list[(regex_replace(splittedString[(int)splittedString.size()-1], std::regex("\\.png"), ""))] = LoadTextureFromImage(LoadImage(fileList[i].c_str()));
        }
    }
}

Texture2D Textures::get(string name) {
    
    return list[name];
}

std::vector<std::string> Textures::SplitString(const string& str, const string& regex_str) {
    std::regex regexz(regex_str);
    std::vector<std::string> list(
        std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
        std::sregex_token_iterator()
    );
    return list;
}

