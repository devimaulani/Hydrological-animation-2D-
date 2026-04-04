#include "menu_screen.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "../../src/wrapper/draw_rectangle.h"
#include "../../src/wrapper/draw_text.h"
#include "../../src/wrapper/draw_circle.h"
#include "../../src/wrapper/draw_ellipse.h"
#include "../../src/objects/cloud.h"
#include "../../src/wrapper/draw_line.h"
#include <math.h>

static Button btnAnimation;
static Button btnRainInfo;
static Button btnShapeInfo;
static Button btnAbout;
static RenderTexture2D menuStaticTex;
static bool menuBaked = false;

static void BakeMenuBackground() {
    menuStaticTex = LoadRenderTexture(SCREEN_W, SCREEN_H);
    BeginTextureMode(menuStaticTex);
        ClearBackground(BLANK);
        
        // 1. Langit Gradasi (Manual Scanline - Dipanggil SEKALI saat Baking)
        for (int y = 0; y < SCREEN_H; y += 4) {
            float t = (float)y / SCREEN_H;
            Color skyCol = (Color){ (unsigned char)(100 + t*50), (unsigned char)(150 + t*100), (unsigned char)(255), 255 };
            Wrapper_DrawRectangleFilled(0, y, SCREEN_W, 4, skyCol);
        }

        // 2. Laut di Bawah (Scanline)
        Wrapper_DrawRectangleFilled(0, SCREEN_H - 120, SCREEN_W, 120, (Color){40, 90, 160, 255});
        
        // 3. Dark Overlay semi-transparan kustom (Bake ke dalam tekstur dasar)
        Wrapper_DrawRectangleFilled(0, 0, SCREEN_W, SCREEN_H, (Color){ 0, 0, 0, 60 });
    EndTextureMode();
    menuBaked = true;
}

void InitMenuScreen() {
    float btnWidth = 350;
    float btnHeight = 60;
    float startX = (SCREEN_W / 2.0f) - (btnWidth / 2.0f);
    float startY = 380;
    float spacing = 75;

    btnAnimation = CreateButton(startX, startY, btnWidth, btnHeight, "1. Animasi Hidrologi");
    btnRainInfo  = CreateButton(startX, startY + spacing, btnWidth, btnHeight, "2. Siklus Hujan");
    btnShapeInfo = CreateButton(startX, startY + spacing * 2, btnWidth, btnHeight, "3. Bentuk Dasar");
    btnAbout     = CreateButton(startX, startY + spacing * 3, btnWidth, btnHeight, "4. About");

    if (!menuBaked) BakeMenuBackground();
}

void DrawMenuScreen() {
    float time = (float)GetTime();

    // 1. Tampilkan Latar Belakang Hasil Baking (Cepat & Tanpa Lag)
    Rectangle source = { 0, 0, (float)menuStaticTex.texture.width, -(float)menuStaticTex.texture.height };
    Rectangle dest = { 0, 0, (float)SCREEN_W, (float)SCREEN_H };
    DrawTexturePro(menuStaticTex.texture, source, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);

    // 2. ELEMEN DINAMIS (Tetap beranimasi lancar di 60 FPS)
    // Matahari Bersinar (Posisi Baru: Menggantikan Gunung)
    float sunX = 300;
    float sunY = 220;
    for (int i = 0; i < 2; i++) {
        float p = fmodf(time * 0.4f + i*0.5f, 1.0f);
        Wrapper_DrawCircleFilled((int)sunX, (int)sunY, 40 + (int)(p * 60), (Color){255, 255, 100, (unsigned char)(80 * (1.0f-p))});
    }
    Wrapper_DrawCircleFilled((int)sunX, (int)sunY, 45, (Color){255, 230, 0, 255});

    // Awan Melayang (Optimasi: Gerak Lebih Tenang & Warna Pekat)
    float cloudX = 280 + sinf(time * 0.4f) * 40;
    DrawCloud(cloudX - 640.0f, 210.0f, 1.0f);

    // 3. Lively Animated Title (Floating)
    Wrapper_DrawTextFloating("HYDROLOGICAL CYCLE", SCREEN_W/2, 160, 64, RAYWHITE, time, 12.0f);
    
    const char *subtitle = "Simulasi Interaktif & Edukasi Grafika Komputer";
    int subSize = 22;
    int subWidth = MeasureText(subtitle, subSize);
    Wrapper_DrawText(subtitle, SCREEN_W/2 - subWidth/2, 240, subSize, (Color){200, 220, 255, 255});

    // 4. Buttons (Sangat responsif di 60 FPS)
    if (DrawButton(&btnAnimation)) currentScreen = SIMULATION_SCREEN;
    if (DrawButton(&btnRainInfo))  currentScreen = RAIN_INFO_SCREEN;
    if (DrawButton(&btnShapeInfo)) currentScreen = SHAPE_INFO_SCREEN;
    if (DrawButton(&btnAbout))     currentScreen = ABOUT_SCREEN;
    
    // 5. Instruction Footer
    Wrapper_DrawText("Gunakan kursor untuk memilih menu", 30, SCREEN_H - 45, 18, (Color){220, 220, 220, 180});
}

void UnloadMenuScreen() {
    if (menuBaked) {
        UnloadRenderTexture(menuStaticTex);
        menuBaked = false;
    }
}
