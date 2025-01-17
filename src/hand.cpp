#include "hand.hpp"
#include <raylib.h>
#include <math.h>
#include <iostream>
#include "global.hpp"
#include <math.h>
#include <raymath.h>
#include <algorithm>

Hand::Hand(bool dontLoad)
{
    position = {100, 100};
    if(!dontLoad) {
        sprite = Textures::shared_instance().get("hand");
        maxFrames = 1;
    }
    col = {0, 0, (float)sprite.width/maxFrames, (float)sprite.height};
    center = {(float)sprite.width/maxFrames/2, (float)sprite.height/2};
    ChangeWeapon(SWORD);
}

void Hand::Update()
{
    curTimer = GetTime();
    float remainingTime = endTimer - curTimer;
    canAttack = remainingTime <= 0;
    switch (curWeapon)
    {
        case SWORD:
            if(IsMouseButtonPressed(0) && canAttack) {
                InitTimer(0.3);
                framesCounter = 0;
                currentFrame = 0;
                animating = true;
                PlaySound(Sounds::shared_instance().get("sword"));
            }
            break;
        case BOW:
            if(IsMouseButtonPressed(0) && canAttack) {
                InitTimer(0.2);
                framesCounter = 0;
                currentFrame = 0;
                animating = true;
                Textures::shared_instance().SpawnProjectile(Projectile({position, angle, 14, "arrow"}));
                PlaySound(Sounds::shared_instance().get("shoot"));
            }
            break;
        default:
            break;
    }
    if(animating) {
        Animate();
    }
}

void Hand::Animate() {
    framesCounter++;

    if (framesCounter >= (60/framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > maxFrames-1) {
            animating = false; 
            currentFrame = 0;
        }

        col.x = (float)currentFrame*(float)sprite.width/maxFrames;
    }

    if(curWeapon == SWORD && currentFrame > 2) {
        for (int i = 0; i < (int)Textures::shared_instance().enemies.size(); i++)
        {
            Rectangle enemyRect = Textures::shared_instance().enemies[i].GetRectangle();
            vector<Vector2> enemyPolygon = {
                { enemyRect.x, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y + enemyRect.height },
                { enemyRect.x, enemyRect.y + enemyRect.height }
            };

            if (Textures::shared_instance().CheckCollisionPolygons(enemyPolygon, GetRotatedRectangle())) {
                Textures::shared_instance().enemies.erase(Textures::shared_instance().enemies.begin() + i);
                PlaySound(Sounds::shared_instance().get("enemyDie"));
                break;
            }
        }
    }
}

void Hand::InitTimer(float time) {
    endTimer = curTimer + time;
}

void Hand::Move(Vector2 pos, Rectangle col, Vector2 center) {
    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), GameManager::shared_instance().camera);

    angle = atan2(mouseWorld.y - pos.y, mouseWorld.x - pos.x);
    float orbitRadius = 30 +(5)*(sprite.width/maxFrames/10)+xOffset;
    position = {pos.x + orbitRadius * cos(angle), pos.y + orbitRadius * sin(angle)};
}

void Hand::ChangeWeapon(Weapon newWeapon) {
    curWeapon = newWeapon;
    switch (newWeapon)
    {
        case HANDS:
            sprite = Textures::shared_instance().get("hand");
            maxFrames = 1;
            xOffset = 0;
            break;
        case SWORD:
            sprite = Textures::shared_instance().get("weapon_sword_animated");
            framesSpeed = 30;
            maxFrames = 7;
            xOffset = -10;
            colHeight = 20;
            colWidth = 31;
            yOffset=20;
            break;
        case BOW:
            sprite = Textures::shared_instance().get("bow_animated");
            framesSpeed = 13;
            maxFrames = 3;
            xOffset = 0;
            break;
        default:
            std::cout<<"Weapon not implemented yet."<<std::endl;
            break;
    }
    center = {(float)sprite.width/maxFrames/2, (float)sprite.height/2};
    col = {0, 0, (float)sprite.width/maxFrames, (float)sprite.height};
}

void Hand::Draw() 
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width/maxFrames, (float)sprite.height}), center, angle * (180 / PI), WHITE);

    if(Textures::shared_instance().DEBUG) {
        auto vertices = GetRotatedRectangle();
        for (int i = 0; i < 4; ++i) {
            DrawLineV(vertices[i], vertices[(i+1) % 4], BLACK);
        }
    }

}

Rectangle Hand::GetRectangle() {
    return {position.x-center.x, position.y-center.y, (float)sprite.width/maxFrames, (float)sprite.height};
}

vector<Vector2> Hand::GetRotatedRectangle() {
    std::vector<Vector2> vertices(4);

    float width = (float)colWidth;
    float height = (float)colHeight;

    Vector2 offsetCenter = {(float)colWidth-yOffset, (float)colHeight+xOffset};

    Vector2 corners[4] = {
        {-offsetCenter.x, -offsetCenter.y},
        {width - offsetCenter.x, -offsetCenter.y},
        {width - offsetCenter.x, height - offsetCenter.y},
        {-offsetCenter.x, height - offsetCenter.y}
    };

    for (int i = 0; i < 4; ++i) {
        vertices[i] = Vector2Rotate(corners[i], angle);
        vertices[i].x += position.x;
        vertices[i].y += position.y;
    }

    return vertices;
}