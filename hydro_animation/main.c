#include "raylib.h"
#include "coords.h"
#include "src/screens/simulation_screen.h"
#include "screen_type.h"

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "Hydrologi Animation");

    G_OriginX = SCREEN_W / 2;
    G_OriginY = SCREEN_H / 2;
    // scaling berdasarkan base
    G_TickStep = (float)SCREEN_W / BASE_WIDTH;
    
    SetTargetFPS(60);

    InitSimulation();

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawSimulation();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}