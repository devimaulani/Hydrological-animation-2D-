#include "raylib.h"
#include "coords.h"
#include "src/screens/simulation_screen.h"
#include "src/screens/menu_screen.h"
#include "src/screens/info_screens.h"
#include "screen_type.h"

Screen currentScreen = MENU_SCREEN;

int main() {
    InitWindow(SCREEN_W, SCREEN_H, "Hydrologi Animation - Tubes Komgraf");

    G_OriginX = SCREEN_W / 2;
    G_OriginY = SCREEN_H / 2;
    G_TickStep = (float)SCREEN_W / BASE_WIDTH;
    
    SetTargetFPS(60);

    // Inisialisasi semua layar yang dibutuhkan
    InitMenuScreen();
    InitSimulation();
    InitInfoScreens();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) {
            case MENU_SCREEN:
                DrawMenuScreen();
                break;
            case SIMULATION_SCREEN:
                DrawSimulation();
                break;
            case RAIN_INFO_SCREEN:
                DrawRainInfoScreen();
                break;
            case SHAPE_INFO_SCREEN:
                DrawShapeInfoScreen();
                break;
            case ABOUT_SCREEN:
                DrawAboutScreen();
                break;
        }

        EndDrawing();
    }

    UnloadMenuScreen();
    UnloadInfoScreens();
    CloseWindow();
    return 0;
}