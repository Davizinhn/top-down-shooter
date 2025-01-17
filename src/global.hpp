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

class Sounds
{
    public:
        static Sounds &shared_instance() {static Sounds sounds; return sounds;}
        map<string, Sound> list;
        map<string, Music> musicList;
        void LoadAll();
        void UnloadAll();
        Sound get(string);
        Music getMusic(string);
    private:
        Sounds();
};

class GameManager
{
    public:
        static GameManager &shared_instance() {static GameManager gameManager; return gameManager;}
        void Update();
        int GetCurrentLevel();
        void Reset();
        Camera2D camera = { 0 };
        float GetMapScale();
        Vector2 playerPos;
    private:
        GameManager();
        int curLevel = 0;
        void NextLevel();
        int timeBetweenLevels = 0;
        int defaultTime = 100;
        int minIncrementValue = 2;
        int maxIncrementValue = 6;
        int levelEnemy = 5;
        bool canProceed = false;
        float mapScale = 5;
};

class Utils
{
    public:
        static Utils &shared_instance() {static Utils utils; return utils;}
        Vector2 Vector2Lerp(Vector2, Vector2, float);
        Color RandomDesaturatedColor();
    private:
        Utils();
};