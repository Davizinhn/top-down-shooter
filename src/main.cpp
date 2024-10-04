#include <raylib.h>
#include "player.hpp"
#include "global.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include <vector>
#include <raymath.h>

enum SceneState {INIT, GAME};

SceneState curScene = INIT;

Player player = Player(true);
Texture2D sampleMap;

bool CheckCollisionPolygons(const std::vector<Vector2>& poly1, const std::vector<Vector2>& poly2) {
    auto checkOverlapOnAxis = [](const Vector2& axis, const std::vector<Vector2>& poly1, const std::vector<Vector2>& poly2) -> bool {
        float min1 = INFINITY, max1 = -INFINITY, min2 = INFINITY, max2 = -INFINITY;

        for (const auto& point : poly1) {
            float projection = Vector2DotProduct(point, axis);
            min1 = fminf(min1, projection);
            max1 = fmaxf(max1, projection);
        }

        for (const auto& point : poly2) {
            float projection = Vector2DotProduct(point, axis);
            min2 = fminf(min2, projection);
            max2 = fmaxf(max2, projection);
        }

        return max1 >= min2 && max2 >= min1;
    };

    std::vector<Vector2> axes;

    for (size_t i = 0; i < poly1.size(); i++) {
        Vector2 edge = Vector2Subtract(poly1[(i + 1) % poly1.size()], poly1[i]);
        axes.push_back(Vector2Normalize({-edge.y, edge.x}));
    }

    for (size_t i = 0; i < poly2.size(); i++) {
        Vector2 edge = Vector2Subtract(poly2[(i + 1) % poly2.size()], poly2[i]);
        axes.push_back(Vector2Normalize({-edge.y, edge.x}));
    }

    for (const auto& axis : axes) {
        if (!checkOverlapOnAxis(axis, poly1, poly2)) {
            return false;
        }
    }

    return true;
}

void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            sampleMap = Textures::shared_instance().get("sampleMap");
            player = Player();
            Textures::shared_instance().SpawnEnemy({});
            break;
        default:
            break;
    }
}

void ProjectileCollision() {
    for (int i = 0; i < (int)Textures::shared_instance().projectiles.size(); i++)
    {
        for (int j = 0; j < (int)Textures::shared_instance().enemies.size(); j++) {
            auto projectilePolygon = Textures::shared_instance().projectiles[i].GetRotatedRectangle();
            Rectangle enemyRect = Textures::shared_instance().enemies[j].GetRectangle();
            std::vector<Vector2> enemyPolygon = {
                { enemyRect.x, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y + enemyRect.height },
                { enemyRect.x, enemyRect.y + enemyRect.height }
            };

            if (CheckCollisionPolygons(enemyPolygon, projectilePolygon)) {
                Textures::shared_instance().enemies.erase(Textures::shared_instance().enemies.begin() + j);
                Textures::shared_instance().projectiles.erase(Textures::shared_instance().projectiles.begin() + i);
            }
        }
    }
}

void Update() {
    switch(curScene) { 
        case GAME:
            // Game scene update

            player.Update();
            
            for (auto &&enemy : Textures::shared_instance().enemies) 
            {
                enemy.Update(player.GetPosition());
            }

            for (auto &&projec : Textures::shared_instance().projectiles)
            {
                projec.Update();
            }

            ProjectileCollision();

            break;
        default:
            break;
    }
}

void Draw() {
    switch(curScene) { 
        case GAME:
            // Game scene draw

            DrawTexture(sampleMap, 0, 0, WHITE);

            for (auto &&enemy : Textures::shared_instance().enemies) 
            {
                enemy.Draw();
            }
            player.Draw();

            for (auto &&projec : Textures::shared_instance().projectiles)
            {
                projec.Draw();
            }

            DrawFPS(0,0);
            break;
        default:
            break;
    }
}

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Top-Down Shooter");
    SetTargetFPS(60);
    Textures::shared_instance().LoadAll();
    ChangeScene(GAME);
    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground({0,0,0,255});        
        Draw();
        EndDrawing();
    }
    Textures::shared_instance().UnloadAll();
    CloseWindow();
    return 0;
}