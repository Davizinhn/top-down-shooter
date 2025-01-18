#pragma once
#include <raylib.h>
#include "hand.hpp"

class Player
{
public:
    Player(bool=false);
    void Update(Vector2 minPos, Vector2 maxPos);
    void Draw();
    Vector2 GetPosition();
    Hand hand = Hand(true);
    Rectangle GetRectangle();
    int GetLives();
    void TakeDamage();
    void SetPosition(Vector2 pos);
    void Regen();

private:
    Vector2 position;
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
    float moveSpeed = 5;
    Vector2 curVelocity = {0, 0};
    float Clamp(float, float, float);
    int lives = 5;
    bool canTakeDamage = true;
    int stunTime = 0;
    int regenTime = 0;
};