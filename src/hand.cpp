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
    }
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
}

void Hand::Update()
{

}

void Hand::Move(Vector2 pos, Rectangle col, Vector2 center) {
    angle = atan2(GetMousePosition().y - pos.y, GetMousePosition().x - pos.x);
    float orbitRadius = 30 +(5)*(sprite.width/10);
    position = {pos.x + orbitRadius * cos(angle), pos.y + orbitRadius * sin(angle)};
}

void Hand::ChangeWeapon(Weapon newWeapon) {
    curWeapon = newWeapon;
    switch (newWeapon)
    {
        case HANDS:
            sprite = Textures::shared_instance().get("hand");
            break;
        case SWORD:
            sprite = Textures::shared_instance().get("weapon_sword");
            break;
        default:
            std::cout<<"Weapon not implemented yet."<<std::endl;
            break;
    }
    center = {(float)sprite.width/2, (float)sprite.height/2};
    col = {0, 0, (float)sprite.width, (float)sprite.height};
}

void Hand::Draw() 
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, angle * (180 / PI), WHITE);
}