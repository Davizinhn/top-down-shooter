#include "hand.hpp"
#include <raylib.h>

Hand::Hand(bool dontLoad)
{
    position = {100, 100};
    if(!dontLoad) {
        sprite = LoadTextureFromImage(LoadImage("resources/player/hand.png"));
    }
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
}

void Hand::Update()
{

}

void Hand::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, 0, WHITE);
}