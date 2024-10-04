#include <raylib.h>
#include "player.hpp"
#include "global.hpp"
#include "enemy.hpp"
#include "projectile.hpp"

enum SceneState {INIT, GAME};

SceneState curScene = INIT;

Player player = Player(true);
//Enemy enemy = Enemy(true);
Texture2D sampleMap;

void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            sampleMap = Textures::shared_instance().get("sampleMap");
            player = Player();
            //enemy = Enemy();
            break;
        default:
            break;
    }
}

void Update() {
    switch(curScene) { 
        case GAME:
            // Game scene update

            player.Update();
            //enemy.Update(player.GetPosition());

            for (auto &&projec : Textures::shared_instance().projectiles)
            {
                projec.Update();
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

            //enemy.Draw();
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