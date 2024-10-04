#pragma once
#include <raylib.h>
#include <string>

using namespace std;

class Projectile
{
public:
    Projectile(Vector2, float, float, string);
    void Update();
    void Draw();
    Vector2 GetPosition();

private:
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
    float velocity = 5;
    float angle = 0;
    Vector2 position;
};