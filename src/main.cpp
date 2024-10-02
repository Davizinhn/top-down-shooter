#include <raylib.h>
#include "player.hpp"

enum SceneState {GAME};

SceneState curScene = GAME;

void ChangeScene(SceneState newScene) {
    if(curScene == newScene)
        return;

    curScene = newScene;

    switch(newScene) { 
        case GAME:
            // Game scene start
            break;
    }
}

void Update() {
    switch(curScene) { 
        case GAME:
            // Game scene update
            break;
    }
}

void Draw() {
    switch(curScene) { 
        case GAME:
            // Game scene draw
            break;
    }
}

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Top-Down Shooter");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground({0,0,0,255});        
        Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}