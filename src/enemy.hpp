#pragma once
#include <raylib.h>
#include <vector>

using namespace std;

class Enemy
{
    public:
        Enemy(Vector2);
        void Update(Vector2, Rectangle);
        void Draw();
        Vector2 GetPosition();
        int life = 3;
        Rectangle GetRectangle();

    private:
        Vector2 position;
        Rectangle col;
        Vector2 center;
        Texture2D sprite; 
        float moveSpeed = 1;
        Vector2 curVelocity = {0, 0};
        float Clamp(float, float, float);
};