#pragma once
#include <raylib.h>

class Enemy
{
    public:
        Enemy(bool=false);
        void Update(Vector2);
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