#include "player.hpp"
#include <raylib.h>
#include "hand.hpp"
#include "global.hpp"

Player::Player(bool dontLoad)
{
    position = {100, 100};
    if(!dontLoad) {
        sprite = LoadTextureFromImage(LoadImage("resources/player/body.png"));
    }
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
    hand = Hand(dontLoad);
}

void Player::Update()
{
    #pragma region Movement Logic
    // Movement logic
    if(IsKeyDown(KEY_W)) {
        curVelocity.y-=moveSpeed;
    } if(IsKeyDown(KEY_S)) {
        curVelocity.y+=moveSpeed;
    } if(IsKeyDown(KEY_D)) {
        curVelocity.x+=moveSpeed;
    } if(IsKeyDown(KEY_A)) {
        curVelocity.x-=moveSpeed;
    }

    float clampValue = curVelocity.x != 0 && curVelocity.y != 0 ? 2.5 : 3;
    curVelocity = {Clamp(curVelocity.x, -clampValue, clampValue), Clamp(curVelocity.y, -clampValue, clampValue)};

    position = {position.x+curVelocity.x, position.y+curVelocity.y};

    curVelocity = {0, 0};
    #pragma endregion

    hand.Move(position, col, center);

    if(IsKeyPressed(KEY_C)) {
        hand.ChangeWeapon(hand.curWeapon == HANDS ? SWORD : HANDS);
    }
}

void Player::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, 0, WHITE);
    hand.Draw();
}

float Player::Clamp(float n, float lower, float upper) {
    n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

Vector2 Player::GetPosition() {
    return position;
}