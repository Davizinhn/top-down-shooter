#pragma once
#include <raylib.h>

class Hand
{
public:
    Hand(bool=false);
    void Update();
    void Draw();
    Vector2 position;

private:
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
};