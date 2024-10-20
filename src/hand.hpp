#pragma once
#include <raylib.h>
#include "global.hpp"
#include <algorithm>

class Hand
{
public:
    Hand(bool=false);
    void Update();
    void Move(Vector2, Rectangle, Vector2);
    void Draw();
    void ChangeWeapon(Weapon);
    Weapon curWeapon = HANDS;
    Vector2 position;
    bool attacking;
    Rectangle GetRectangle();
    vector<Vector2> GetRotatedRectangle();

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
    float yOffset = 0;

    int colHeight = 0;
    int colWidth = 0;
};