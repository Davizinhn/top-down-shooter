#pragma once

#include <raylib.h>
#include <iostream>
#include <map>
#include "projectile.hpp"
#include <vector>
#include "enemy.hpp"
using namespace std;

enum Weapon {HANDS, SWORD, BOW};
class Textures
{
    public:
        bool DEBUG = false;
        static Textures &shared_instance() {static Textures textures; return textures;}
        map<string, Texture2D> list;
        void LoadAll();
        void UnloadAll();
        Texture2D get(string);
        std::vector<std::string> SplitString(const string&, const string&);
        void SpawnProjectile(Projectile);
        void SpawnEnemy(Enemy);
        vector<Projectile> projectiles;
        vector<Enemy> enemies;
        bool CheckCollisionPolygons(const vector<Vector2>&, const vector<Vector2>&);
        float RandomFloat(float, float);
        Vector2 RandomVector2(Vector2, Vector2);
    private:
        Textures();
};