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

Color randomColor;

void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            randomColor = Utils::shared_instance().RandomDesaturatedColor();
            sampleMap = Textures::shared_instance().get("sampleMap");
            player = Player();
            player.SetPosition({(float)GetRandomValue(0, sampleMap.width*GameManager::shared_instance().GetMapScale()), (float)GetRandomValue(0, sampleMap.height*GameManager::shared_instance().GetMapScale())});
            cursor = Textures::shared_instance().get("cursor");
            cRect = {0, 0, (float)cursor.width, (float)cursor.height};
            heartTexture = Textures::shared_instance().get("heart");
            GameManager::shared_instance().Reset();
            HideCursor();

            curMusic = Sounds::shared_instance().getMusic("music"+to_string(GetRandomValue(1, 5)));
            PlayMusicStream(curMusic);

            GameManager::shared_instance().camera.target = player.GetPosition();
            GameManager::shared_instance().camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
            GameManager::shared_instance().camera.rotation = 0.0f;
            GameManager::shared_instance().camera.zoom = 1.0f;

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

void UpdateCamera() {
    Vector2 camVec = Utils::shared_instance().Vector2Lerp(GameManager::shared_instance().camera.target, player.GetPosition(), 8 * GetFrameTime());

    float mapWidth = sampleMap.width * GameManager::shared_instance().GetMapScale();
    float mapHeight = sampleMap.height * GameManager::shared_instance().GetMapScale();
    float offsetX = GameManager::shared_instance().camera.offset.x;
    float offsetY = GameManager::shared_instance().camera.offset.y;

    GameManager::shared_instance().camera.target = {
        Clamp(camVec.x, offsetX, mapWidth - offsetX),
        Clamp(camVec.y, offsetY, mapHeight - offsetY)
    };
}

void Update() {
    switch(curScene) { 
        case GAME:
            // Game scene update

            UpdateMusicStream(curMusic);

            player.Update({0, 0}, {(float)sampleMap.width*GameManager::shared_instance().GetMapScale(), (float)sampleMap.height*GameManager::shared_instance().GetMapScale()});
            GameManager::shared_instance().playerPos = player.GetPosition();
            
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

            UpdateCamera();

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

void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint)
{
    if ((texture.id <= 0) || (scale <= 0.0f)) return;  // Wanna see a infinite loop?!...just delete this line!
    if ((source.width == 0) || (source.height == 0)) return;

    int tileWidth = (int)(source.width*scale), tileHeight = (int)(source.height*scale);
    if ((dest.width < tileWidth) && (dest.height < tileHeight))
    {
        // Can fit only one tile
        DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
                    Rectangle{dest.x, dest.y, dest.width, dest.height}, origin, rotation, tint);
    }
    else if (dest.width <= tileWidth)
    {
        // Tiled vertically (one column)
        int dy = 0;
        for (;dy+tileHeight < dest.height; dy += tileHeight)
        {
            DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)dest.width/tileWidth)*source.width, source.height}, Rectangle{dest.x, dest.y + dy, dest.width, (float)tileHeight}, origin, rotation, tint);
        }

        // Fit last tile
        if (dy < dest.height)
        {
            DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                        Rectangle{dest.x, dest.y + dy, dest.width, dest.height - dy}, origin, rotation, tint);
        }
    }
    else if (dest.height <= tileHeight)
    {
        // Tiled horizontally (one row)
        int dx = 0;
        for (;dx+tileWidth < dest.width; dx += tileWidth)
        {
            DrawTexturePro(texture, Rectangle{source.x, source.y, source.width, ((float)dest.height/tileHeight)*source.height}, Rectangle{dest.x + dx, dest.y, (float)tileWidth, dest.height}, origin, rotation, tint);
        }

        // Fit last tile
        if (dx < dest.width)
        {
            DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
                        Rectangle{dest.x + dx, dest.y, dest.width - dx, dest.height}, origin, rotation, tint);
        }
    }
    else
    {
        // Tiled both horizontally and vertically (rows and columns)
        int dx = 0;
        for (;dx+tileWidth < dest.width; dx += tileWidth)
        {
            int dy = 0;
            for (;dy+tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, source, Rectangle{dest.x + dx, dest.y + dy, (float)tileWidth, (float)tileHeight}, origin, rotation, tint);
            }

            if (dy < dest.height)
            {
                DrawTexturePro(texture, Rectangle{source.x, source.y, source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                    Rectangle{dest.x + dx, dest.y + dy, (float)tileWidth, dest.height - dy}, origin, rotation, tint);
            }
        }

        // Fit last column of tiles
        if (dx < dest.width)
        {
            int dy = 0;
            for (;dy+tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, source.height},
                        Rectangle{dest.x + dx, dest.y + dy, dest.width - dx, (float)tileHeight}, origin, rotation, tint);
            }

            // Draw final tile in the bottom right corner
            if (dy < dest.height)
            {
                DrawTexturePro(texture, Rectangle{source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                    Rectangle{dest.x + dx, dest.y + dy, dest.width - dx, dest.height - dy}, origin, rotation, tint);
            }
        }
    }
}

void Draw() {
    switch(curScene) { 
        case GAME:
            // Game scene draw
            BeginMode2D(GameManager::shared_instance().camera);

            DrawTextureTiled(sampleMap, {0, 0, (float)sampleMap.width* GameManager::shared_instance().GetMapScale(), (float)sampleMap.height * GameManager::shared_instance().GetMapScale()}, {0, 0, (float)sampleMap.width * GameManager::shared_instance().GetMapScale(), (float)sampleMap.height * GameManager::shared_instance().GetMapScale()}, {0, 0}, 0, 0.5, randomColor);

            for (auto &&enemy : Textures::shared_instance().enemies) 
            {
                enemy.Draw();
            }
            player.Draw();

            for (auto &&projec : Textures::shared_instance().projectiles)
            {
                projec.Draw();
            }

            EndMode2D();

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
    const int screenWidth = 1280;
    const int screenHeight = 720;

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
    Sounds::shared_instance().UnloadAll();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}