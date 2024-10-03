#pragma once

#include <raylib.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

enum Weapon {HANDS, SWORD};
class Textures
{
    public:
        static Textures &shared_instance() {static Textures textures; return textures;}
        map<string, Texture2D> list;
        void LoadAll();
        void UnloadAll();
        Texture2D get(string);
        std::vector<std::string> SplitString(const string&, const string&);
    private:
        Textures();
};