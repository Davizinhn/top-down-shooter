#include "enemy.hpp"
#include <raylib.h>
#include "global.hpp"

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
    curVelocity={curVelocity.x+(playerPosition.x-position.x), curVelocity.y+(playerPosition.y-position.y)};

    float clampValue = curVelocity.x != 0 && curVelocity.y != 0 ? 0.7: 1;
    curVelocity = {Clamp(curVelocity.x, -clampValue, clampValue), Clamp(curVelocity.y, -clampValue, clampValue)};

    position = {position.x+curVelocity.x, position.y+curVelocity.y};

    curVelocity = {0, 0};
}

void Enemy::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, 0, WHITE);
}

float Enemy::Clamp(float n, float lower, float upper) {
    n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

Vector2 Enemy::GetPosition() {
    return position;
}