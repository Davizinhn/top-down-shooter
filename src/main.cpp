#include <raylib.h>
#include "player.hpp"
#include "global.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include <vector>
#include <raymath.h>

enum SceneState {INIT, MAINMENU, GAME, GAMEOVER};

SceneState curScene = INIT;

Player player = Player(true);
Texture2D sampleMap;

Rectangle cRect;
Texture2D cursor;
Texture2D heartTexture;

Music curMusic;

void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            sampleMap = Textures::shared_instance().get("sampleMap");
            player = Player();
            cursor = Textures::shared_instance().get("cursor");
            cRect = {0, 0, (float)cursor.width, (float)cursor.height};
            heartTexture = Textures::shared_instance().get("heart");
            GameManager::shared_instance().Reset();
            HideCursor();
            curMusic = Sounds::shared_instance().getMusic("music"+to_string(GetRandomValue(1, 5)));
            PlayMusicStream(curMusic);
            break;
        case GAMEOVER:
            PlaySound(Sounds::shared_instance().get("playerDie"));
            StopMusicStream(curMusic);
            ShowCursor();
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
                PlaySound(Sounds::shared_instance().get("enemyDie"));
                break;
            }
        }

        if(Vector2Distance(Textures::shared_instance().projectiles[i].initialPos, Textures::shared_instance().projectiles[i].GetPosition()) > 900) {
            Textures::shared_instance().projectiles.erase(Textures::shared_instance().projectiles.begin() + i);
        }
    }

    for (int j = 0; j < (int)Textures::shared_instance().enemies.size(); j++) {
        Enemy curEnemy = Textures::shared_instance().enemies[j];
        if(CheckCollisionRecs(curEnemy.GetRectangle(), player.GetRectangle())) {
            Textures::shared_instance().enemies.erase(Textures::shared_instance().enemies.begin() + j);
            player.TakeDamage();
        }
    }
}

void Update() {
    switch(curScene) { 
        case GAME:
            // Game scene update

            UpdateMusicStream(curMusic);

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
            GameManager::shared_instance().Update();

            if(player.GetLives() < 1) {
                ChangeScene(GAMEOVER);
            }

            break;
        case GAMEOVER:
            if(IsKeyPressed(KEY_R)) {
                ChangeScene(GAME);
            }
            break;
        case MAINMENU:
            if(GetKeyPressed()) {
                ChangeScene(GAME);
            }
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

            for(int i = 0; i<player.GetLives(); i++) {
                DrawTexture(heartTexture, 10+(i*35), 10, WHITE);
            }

            if(Textures::shared_instance().DEBUG) {
                DrawText(TextFormat("Enemies: %06i", Textures::shared_instance().enemies.size()), 0, 30, 20, BLACK);
                DrawText(TextFormat("Projectiles: %06i", Textures::shared_instance().projectiles.size()), 0, 60, 20, BLACK);
            }

            DrawText(TextFormat("Level: %02i", GameManager::shared_instance().GetCurrentLevel()), 2, GetScreenHeight()-20, 20, BLACK);

            DrawTexturePro(cursor, cRect, {GetMousePosition().x, GetMousePosition().y, cRect.width, cRect.height}, {cRect.width/2, cRect.height/2}, 0, WHITE);

            break;
        case GAMEOVER:
            DrawText(TextFormat("Levels Survived: %02i", GameManager::shared_instance().GetCurrentLevel() > 0 ? GameManager::shared_instance().GetCurrentLevel()-1 : 0), 20, 55, 35, WHITE);
            DrawText("GAME OVER", 20, 20, 35, WHITE);
            DrawText("Press R to retry", 20, GetScreenHeight()-45, 35, WHITE);
            break;
        case MAINMENU:
            DrawText("Top-Down Shooter", 20, 20, 35, WHITE);
            DrawText("Press anything to play", 20, GetScreenHeight()-45, 35, WHITE);
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
    InitAudioDevice();
    SetTargetFPS(60);
    Textures::shared_instance().LoadAll();
    Sounds::shared_instance().LoadAll();
    ChangeScene(MAINMENU);
    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground({0,0,0,255});        
        Draw();
        EndDrawing();
    }
    Textures::shared_instance().UnloadAll();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}