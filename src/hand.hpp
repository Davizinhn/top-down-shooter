#pragma once
#include <raylib.h>

class Hand
{
public:
    enum Weapon {HANDS, SWORD};
    Hand(bool=false);
    void Update();
    void Move(Vector2, Rectangle, Vector2);
    void Draw();
    void ChangeWeapon(Weapon);
    Weapon curWeapon = HANDS;
    Vector2 position;

private:
    Rectangle col;
    Vector2 center;
    Texture2D sprite; 
    float angle;
};