#include "enemy.hpp"
#include <raylib.h>
#include "global.hpp"
#include <math.h>
#include "player.hpp"
using namespace std;

Enemy::Enemy(Vector2 pos)
{
    position = pos;
    sprite = Textures::shared_instance().get("body");
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
}

void Enemy::Update(Vector2 playerPosition, Rectangle playerRect)
{
    Vector2 direction = {playerPosition.x - position.x, playerPosition.y - position.y};
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }
    position = {position.x + direction.x * moveSpeed, position.y + direction.y * moveSpeed};

    for (Enemy& other : Textures::shared_instance().enemies) {
        if (&other == this) continue;
        float distance = sqrt((other.position.x - position.x) * (other.position.x - position.x) + (other.position.y - position.y) * (other.position.y - position.y));

        float minDistance = (sprite.width + other.sprite.width) / 2;

        if (distance < minDistance) {
            Vector2 separationDirection = {position.x - other.position.x, position.y - other.position.y};
            float separationMagnitude = sqrt(separationDirection.x * separationDirection.x + separationDirection.y * separationDirection.y);
            if (separationMagnitude > 0) {
                separationDirection.x /= separationMagnitude;
                separationDirection.y /= separationMagnitude;
            }
            position.x += separationDirection.x * (minDistance - distance) / 2;
            position.y += separationDirection.y * (minDistance - distance) / 2;
        }
    }

    if (CheckCollisionRecs(GetRectangle(), playerRect)) {
        Vector2 separationDirection = {position.x - playerPosition.x, position.y - playerPosition.y};
        float separationMagnitude = sqrt(separationDirection.x * separationDirection.x + separationDirection.y * separationDirection.y);
        if (separationMagnitude > 0) {
            separationDirection.x /= separationMagnitude;
            separationDirection.y /= separationMagnitude;
        }
        position.x += separationDirection.x * moveSpeed;
        position.y += separationDirection.y * moveSpeed;
    }
}

void Enemy::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, 0, RED);
    if(Textures::shared_instance().DEBUG) {
        DrawRectangleLinesEx(GetRectangle(), 3, BLACK);
    }
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