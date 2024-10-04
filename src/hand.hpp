#pragma once
#include <raylib.h>
#include "global.hpp"

class Hand
{
public:
    Hand(bool=false);
    void Update();
    void Move(Vector2, Rectangle, Vector2);
    void Draw();
    void ChangeWeapon(Weapon);
    Weapon curWeapon = SWORD;
    Vector2 position;
    bool attacking;

private:
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
    float angle;
    void InitTimer(float);
    void Animate();
    float curTimer = 0;
    float endTimer = 0;
    bool canAttack = true;

    int framesCounter = 0;
    int framesSpeed = 35;
    int currentFrame = 0;
    int maxFrames = 0;
    bool animating = false;
    float xOffset = 0;
};