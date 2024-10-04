#include "projectile.hpp"
#include <raylib.h>
#include "global.hpp"
#include <math.h>
#include <string>
using namespace std;

Projectile::Projectile(Vector2 pos, float ang, float vel, string tex)
{
    position = pos;
    angle = ang;
    velocity = vel;
    sprite = Textures::shared_instance().get(tex);
    col = {0, 0, (float)sprite.width, (float)sprite.height};
    center = {(float)sprite.width/2, (float)sprite.height/2};
}

void Projectile::Update()
{
    position = {position.x+velocity*cos(angle), position.y+velocity*sin(angle)};
}


void Projectile::Draw()
{
    DrawTexturePro(sprite, col, Rectangle({position.x, position.y, (float)sprite.width, (float)sprite.height}), center, angle * (180 / PI), WHITE);
}

Vector2 Projectile::GetPosition() {
    return position;
}