#include "enemy.hpp"
#include <raylib.h>
#include "global.hpp"
#include <math.h>

Enemy::Enemy(bool dontLoad)
{
    position = {100, 100};
    if(!dontLoad) {
        sprite = Textures::shared_instance().get("body");
    }
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
}

void Enemy::Update(Vector2 playerPosition)
{
    Vector2 direction = {playerPosition.x - position.x, playerPosition.y - position.y};

    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    position = {position.x + direction.x * moveSpeed, position.y + direction.y * moveSpeed};
}

void Enemy::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, 0, RED);
    //DrawRectangleLinesEx(GetRectangle(), 3, WHITE);
}

float Enemy::Clamp(float n, float lower, float upper) {
    n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

Vector2 Enemy::GetPosition() {
    return position;
}

Rectangle Enemy::GetRectangle() {
    return {position.x-center.x, position.y-center.y, (float)sprite.width, (float)sprite.height};
}