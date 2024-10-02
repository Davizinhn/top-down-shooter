#pragma once
#include <raylib.h>

class Player
{
public:
    Player();
    void Update();
    void Draw();

private:
    Vector2 position;
};