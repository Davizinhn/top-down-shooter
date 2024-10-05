#include "projectile.hpp"
#include <raylib.h>
#include "global.hpp"
#include <math.h>
#include <string>
#include <algorithm>
#include <raymath.h>
using namespace std;

Projectile::Projectile(Vector2 pos, float ang, float vel, string tex)
{
    position = pos;
    initialPos = pos;
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
    if(Textures::shared_instance().DEBUG) {
        auto vertices = GetRotatedRectangle();
        for (int i = 0; i < 4; ++i) {
            DrawLineV(vertices[i], vertices[(i+1) % 4], BLACK);
        }
    }
}

Vector2 Projectile::GetPosition() {
    return position;
}

vector<Vector2> Projectile::GetRotatedRectangle() {
    std::vector<Vector2> vertices(4);

    float width = (float)sprite.width;
    float height = (float)sprite.height;

    Vector2 corners[4] = {
        {-center.x, -center.y},
        {width - center.x, -center.y},
        {width - center.x, height - center.y},
        {-center.x, height - center.y}
    };

    for (int i = 0; i < 4; ++i) {
        vertices[i] = Vector2Rotate(corners[i], angle);
        vertices[i].x += position.x;
        vertices[i].y += position.y;
    }

    return vertices;
}