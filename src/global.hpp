#pragma once

#include <raylib.h>
#include <iostream>
#include <map>
#include "projectile.hpp"
#include <vector>
using namespace std;

enum Weapon {HANDS, SWORD, BOW};
class Textures
{
    public:
        static Textures &shared_instance() {static Textures textures; return textures;}
        map<string, Texture2D> list;
        void LoadAll();
        void UnloadAll();
        Texture2D get(string);
        std::vector<std::string> SplitString(const string&, const string&);
        void SpawnProjectile(Projectile);
        vector<Projectile> GetProjectiles();
    private:
        Textures();
        vector<Projectile> projectiles;
};