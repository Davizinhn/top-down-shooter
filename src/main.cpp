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

Rectangle cRect;
Texture2D cursor;


void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            sampleMap = Textures::shared_instance().get("sampleMap");
            player = Player();
            for(int i = 0; i < GetRandomValue(1,60); i++) {
                Textures::shared_instance().SpawnEnemy({Textures::shared_instance().RandomVector2({1000,0}, {1000,600})});
            }
            for(int i = 0; i < GetRandomValue(1,60); i++) {
                Textures::shared_instance().SpawnEnemy({Textures::shared_instance().RandomVector2({0,1000}, {800,1000})});
            }
            cursor = Textures::shared_instance().get("cursor");
            cRect = {0, 0, (float)cursor.width, (float)cursor.height};
            HideCursor();
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
            vector<Vector2> enemyPolygon = {
                { enemyRect.x, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y },
                { enemyRect.x + enemyRect.width, enemyRect.y + enemyRect.height },
                { enemyRect.x, enemyRect.y + enemyRect.height }
            };

            if (Textures::shared_instance().CheckCollisionPolygons(enemyPolygon, projectilePolygon)) {
                Textures::shared_instance().enemies.erase(Textures::shared_instance().enemies.begin() + j);
                Textures::shared_instance().projectiles.erase(Textures::shared_instance().projectiles.begin() + i);
                break;
            }
        }

        if(Vector2Distance(Textures::shared_instance().projectiles[i].initialPos, Textures::shared_instance().projectiles[i].GetPosition()) > 900) {
            Textures::shared_instance().projectiles.erase(Textures::shared_instance().projectiles.begin() + i);
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
                enemy.Update(player.GetPosition(), player.GetRectangle());
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

            if(Textures::shared_instance().DEBUG) {
                DrawText(TextFormat("Enemies: %06i", Textures::shared_instance().enemies.size()), 0, 30, 20, BLACK);
                DrawText(TextFormat("Projectiles: %06i", Textures::shared_instance().projectiles.size()), 0, 60, 20, BLACK);
            }

            DrawTexturePro(cursor, cRect, {GetMousePosition().x, GetMousePosition().y, cRect.width, cRect.height}, {cRect.width/2, cRect.height/2}, 0, WHITE);

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