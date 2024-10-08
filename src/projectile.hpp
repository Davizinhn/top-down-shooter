#pragma once
#include <raylib.h>
#include <string>
#include <vector>

using namespace std;

class Projectile
{
    public:
        Projectile(Vector2, float, float, string);
        void Update();
        void Draw();
        Vector2 GetPosition();
        vector<Vector2> GetRotatedRectangle();
        Vector2 initialPos;

    private:
        Rectangle col;
        Vector2 center;
        Texture2D sprite; 
        float velocity = 5;
        float angle = 0;
        Vector2 position;
};