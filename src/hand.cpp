#include "hand.hpp"
#include <raylib.h>
#include <math.h>
#include <iostream>
#include "global.hpp"

Hand::Hand(bool dontLoad)
{
    position = {100, 100};
    if(!dontLoad) {
        sprite = Textures::shared_instance().get("hand");
        maxFrames = 1;
    }
    col = {0, 0, (float)sprite.width/maxFrames, (float)sprite.height};
    center = {(float)sprite.width/maxFrames/2, (float)sprite.height/2};
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
            }
            break;
        case BOW:
            if(IsMouseButtonPressed(0) && canAttack) {
                InitTimer(0.45);
                Textures::shared_instance().SpawnProjectile(Projectile({position, angle, 8, "arrow"}));
                framesCounter = 0;
                currentFrame = 0;
                animating = true;
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
}

void Hand::InitTimer(float time) {
    endTimer = curTimer + time;
}

void Hand::Move(Vector2 pos, Rectangle col, Vector2 center) {
    angle = atan2(GetMousePosition().y - pos.y, GetMousePosition().x - pos.x);
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
            break;
        case BOW:
            sprite = Textures::shared_instance().get("bow_animated");
            framesSpeed = 10;
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
}