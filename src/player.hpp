#pragma once
#include <raylib.h>
#include "hand.hpp"

class Player
{
public:
    Player(bool=false);
    void Update();
    void Draw();
    Vector2 GetPosition();
    Hand hand = Hand(true);
    Rectangle GetRectangle();

private:
    Vector2 position;
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
    float moveSpeed = 5;
    Vector2 curVelocity = {0, 0};
    float Clamp(float, float, float);
};