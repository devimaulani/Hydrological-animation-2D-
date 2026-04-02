#include "menu_screen.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "raylib.h"

static Texture2D menuBg;
static Button btnAnimation;
static Button btnRainInfo;
static Button btnShapeInfo;
static Button btnAbout;

void InitMenuScreen() {
    menuBg = LoadTexture("assets/menu_bg.png");
    
    float btnWidth = 350;
    float btnHeight = 60;
    float startX = (SCREEN_W / 2.0f) - (btnWidth / 2.0f);
    float startY = 400;
    float spacing = 80;

    btnAnimation = CreateButton(startX, startY, btnWidth, btnHeight, "1. Animasi Hidrologi");
    btnRainInfo  = CreateButton(startX, startY + spacing, btnWidth, btnHeight, "2. Siklus Hujan");
    btnShapeInfo = CreateButton(startX, startY + spacing * 2, btnWidth, btnHeight, "3. Bentuk Dasar");
    btnAbout     = CreateButton(startX, startY + spacing * 3, btnWidth, btnHeight, "4. About");
}

void DrawMenuScreen() {
    // Draw Background Layer
    DrawTexturePro(menuBg, 
        (Rectangle){ 0, 0, (float)menuBg.width, (float)menuBg.height },
        (Rectangle){ 0, 0, (float)SCREEN_W, (float)SCREEN_H },
        (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Overlay to make text and buttons pop
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, (Color){ 0, 0, 0, 80 });

    // Title
    const char *title = "HYDROLOGICAL CYCLE";
    int titleSize = 60;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, SCREEN_W/2 - titleWidth/2, 180, titleSize, WHITE);
    
    const char *subtitle = "Simulasi Interaktif & Edukasi Grafika Komputer";
    int subSize = 24;
    int subWidth = MeasureText(subtitle, subSize);
    DrawText(subtitle, SCREEN_W/2 - subWidth/2, 250, subSize, LIGHTGRAY);

    // Buttons
    if (DrawButton(&btnAnimation)) currentScreen = SIMULATION_SCREEN;
    if (DrawButton(&btnRainInfo))  currentScreen = RAIN_INFO_SCREEN;
    if (DrawButton(&btnShapeInfo)) currentScreen = SHAPE_INFO_SCREEN;
    if (DrawButton(&btnAbout))     currentScreen = ABOUT_SCREEN;
    
    // Instructions
    DrawText("Pilih menu untuk memulai", 20, SCREEN_H - 40, 20, GRAY);
}

void UnloadMenuScreen() {
    UnloadTexture(menuBg);
}
