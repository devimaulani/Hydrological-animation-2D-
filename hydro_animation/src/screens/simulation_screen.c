#include "simulation_screen.h"
#include "raylib.h"
#include "../objects/sky.h"
#include "../objects/sun.h"
#include "../objects/mountain.h"
#include "../objects/water.h"
#include "../objects/tree.h"
#include "../objects/land.h"
#include "../objects/cloud.h"
#include "../objects/lake.h"
#include "../objects/rock.h"
#include "../objects/grass.h"
#include "../objects/flower.h"
#include "../animation/evaporation.h"
#include "../animation/transpiration.h"
#include "../simulation/simulation_state.h"
#include "../animation/advection.h"
#include "../animation/precipitation.h"
#include "../animation/infiltration.h"
#include "../animation/collection.h"
#include "../objects/birds.h"
#include "../controls/slider.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "coords.h"
#include <stdlib.h>
#include <math.h>

static Button btnBackSim;

// Modular Textures (Baking)
static RenderTexture2D texMountain;
static RenderTexture2D texLand;
static RenderTexture2D texNature;
static RenderTexture2D texCloud;
static float riverOverflow = 1.0f;

static bool texturesLoaded = false;

static void BakeModularTextures() {
    int oldOX = G_OriginX;
    int oldOY = G_OriginY;

    // 1. Bake Mountains
    texMountain = LoadRenderTexture(SCREEN_W, SCREEN_H);
    BeginTextureMode(texMountain);
        ClearBackground(BLANK);
        DrawMountain();
    EndTextureMode();

    // 2. Bake Land
    texLand = LoadRenderTexture(SCREEN_W, SCREEN_H);
    BeginTextureMode(texLand);
        ClearBackground(BLANK);
        DrawLand();
    EndTextureMode();

    // 3. Bake Lake
    // Sungai digambar secara dinamis

    // 4. Bake Nature (Trees & Rocks)
    texNature = LoadRenderTexture(SCREEN_W, SCREEN_H);
    BeginTextureMode(texNature);
        ClearBackground(BLANK);
        int landTop = (int)(-HALF_H / 4.2f);
        // 1. Gambar Pohon Terlebih Dahulu (Baking)
        DrawRock(LEFT + 550, landTop + 10, 35, 18);
        DrawTreeTall(LEFT + 100, landTop - 45);
        DrawTreeLarge(LEFT + 300, landTop - 65);
        DrawTreeBushy(LEFT + 500, landTop - 130);
        DrawTreeTall(LEFT + 680, landTop - 45);

        // 2. Gambar Batu Statis (Hanya yang tidak menghalangi pohon)
        DrawRock(LEFT + 485, landTop - 133, 35, 18);

    EndTextureMode();

    // 5. Sun digambar dinamik juga

    // 6. Bake Cloud Template (Full Solid)
    texCloud = LoadRenderTexture(250, 150);
    G_OriginX = 125; G_OriginY = 75;
    BeginTextureMode(texCloud);
        ClearBackground(BLANK);
        DrawCloud(0, 0, 1.0f); // Bake at 100% opacity
    EndTextureMode();

    // Restore Origins
    G_OriginX = oldOX;
    G_OriginY = oldOY;
    texturesLoaded = true;
}

static void UnloadSimulationTextures() {
    if (!texturesLoaded) return;
    UnloadRenderTexture(texMountain);
    UnloadRenderTexture(texLand);
    UnloadRenderTexture(texNature);
    UnloadRenderTexture(texCloud);
    // texSun is no longer used
    texturesLoaded = false;
}

void InitSimulation() {
    InitEvaporation();
    InitTranspiration();
    InitAdvection();
    InitRain();
    InitLake();
    InitInfiltration();
    InitCollection();
    InitBirds();
    BakeModularTextures();
    btnBackSim = CreateButton(20, 20, 150, 40, "Menu Utama");
}

static void DrawBakedTexture(RenderTexture2D tex, Color tint) {
    Rectangle source = { 0, 0, (float)tex.texture.width, -(float)tex.texture.height };
    Rectangle dest = { 0, 0, (float)SCREEN_W, (float)SCREEN_H };
    DrawTexturePro(tex.texture, source, dest, (Vector2){ 0, 0 }, 0.0f, tint);
}

static void DrawBakedObject(RenderTexture2D tex, float cartX, float cartY, float scale, Color tint) {
    Rectangle source = { 0, 0, (float)tex.texture.width, -(float)tex.texture.height };
    float screenX = CS_Xf(cartX);
    float screenY = CS_Yf(cartY);
    
    float sw = (float)tex.texture.width * scale;
    float sh = (float)tex.texture.height * scale;
    
    Rectangle dest = { screenX - sw/2, screenY - sh/2, sw, sh };
    DrawTexturePro(tex.texture, source, dest, (Vector2){ 0, 0 }, 0.0f, tint);
}

#include "../wrapper/draw_rectangle.h"

static void DrawPhaseLabel(const char* label) {
    Vector2 mouse = GetMousePosition();
    int textWidth = MeasureText(label, 20);
    // Menggunakan Wrapper kustom alih-alih raylib native
    Wrapper_DrawRectangleFilled(mouse.x + 15, mouse.y + 15, textWidth + 20, 40, (Color){0, 0, 0, 180});
    DrawText(label, mouse.x + 25, mouse.y + 25, 20, RAYWHITE);
}

void DrawSimulation() {
    // PERBAIKAN: Pastikan tekstur di-bake ulang jika sebelumnya telah di-unload
    if (!texturesLoaded) BakeModularTextures();

    float dt = GetFrameTime();
    UpdateSimulationState(dt);

    // Langit dengan transisi bertahap (tidak tiba-tiba berubah)
    DrawSkyGradual(skyDarkness);
    
    // Matahari dinamis dengan smooth fade out saat langit gelap
    DrawSun(sunX, sunY, 45, 1.0f - skyDarkness);
    DrawBakedTexture(texMountain, WHITE);
    DrawBakedTexture(texLand, WHITE);
    
    // 3. Update & Draw Sungai Dinamis (Meluap jika Hujan/Koleksi)
    float targetOverflow = 1.0f;
    if (currentPhase == PHASE_PRECIPITATION) targetOverflow = 1.25f;
    else if (currentPhase == PHASE_INFILTR_COLLECT) targetOverflow = 1.15f;
    
    // Smooth interpolation (Ease Out)
    riverOverflow += (targetOverflow - riverOverflow) * 0.05f;
    DrawLake(riverOverflow);

    DrawWater(-106);
    
    // 4. Update & Draw Aliran Sungai (Collection)
    UpdateCollection(dt);
    DrawCollection();


    // PHASES
    switch (currentPhase) {
        case PHASE_EVAP_TRANSP:
        case PHASE_CONDENSATION:
            UpdateEvaporation(dt);
            UpdateTranspiration(dt);
            DrawEvaporation();
            DrawTranspiration();
            break;
        case PHASE_ADVECTION:
            UpdateAdvection(dt);
            DrawAdvection();
            break;
        case PHASE_PRECIPITATION:
            UpdateAdvection(dt);  // Lanjutkan update angin saat hujan
            DrawAdvection();      // Gambar angin juga saat hujan
            UpdateRain(dt);
            DrawRain();
            break;
        case PHASE_INFILTR_COLLECT:
            UpdateInfiltration(dt);
            DrawInfiltration();
            break;
        default: break;
    }

    DrawBakedTexture(texNature, WHITE);

    // 4.5. Vegetasi Dinamis (Rumput & Bunga yang bergoyang tertiup angin)
    // Tuliskan Kode Pohon Dulu Baru Bunga (Agar bunga menimpa/di depan batang pohon)
    float sway = sinf(GetTime() * 3.5f) * 6.0f;
    int landTop = (int)(-HALF_H / 4.2f);
    
    // Kluster Vegetasi di Pohon 1 (Tall) - Dasar: landTop - 45
    DrawFlower(   LEFT + 80,  landTop - 65, sway * 0.8f);
    DrawGrassTuft(LEFT + 100, landTop - 65, sway);
    DrawFlower(   LEFT + 120, landTop - 65, sway * 1.1f);
    
    // Kluster Vegetasi di Pohon 2 (Large) - Dasar: landTop - 65 
    DrawGrassTuft(LEFT + 260, landTop - 70, sway * 0.8f);
    DrawFlower(   LEFT + 340, landTop - 70, sway * 1.2f);
    DrawFlower(   LEFT + 300, landTop - 70, sway);
    DrawFlower(   LEFT + 280, landTop - 70, sway * 0.9f);
    DrawGrassTuft(LEFT + 320, landTop - 70, sway * 1.1f);
    
    // Kluster Vegetasi di Pohon 3 (Bushy) - Dasar: landTop - 130
    DrawGrassTuft(LEFT + 530, landTop - 130, sway * 1.2f);
    DrawFlower(   LEFT + 545, landTop - 130, sway * 0.9f);
    
    // Kluster Vegetasi di Pohon 4 (Tall) - Dasar: landTop - 45
    DrawGrassTuft(LEFT + 660, landTop - 45, sway);
    DrawFlower(   LEFT + 680, landTop - 45, sway * 1.3f);
    DrawFlower(   LEFT + 700, landTop - 45, sway);

    // DRAW DYNAMIC CLOUDS FROM STATE (With Growing Scale & Gradual Darkening)
    for (int i = 0; i < 2; i++) {
        unsigned char a = (unsigned char)(255 * mainClouds[i].opacity);
        
        // Warna awan bertransisi bertahap berdasarkan darkProgress
        // Putih (255,255,255) -> Abu-abu tua (100,105,120)
        unsigned char r_val = (unsigned char)(255 - (155 * mainClouds[i].darkProgress));
        unsigned char g_val = (unsigned char)(255 - (150 * mainClouds[i].darkProgress));
        unsigned char b_val = (unsigned char)(255 - (135 * mainClouds[i].darkProgress));
        Color cloudTint = (Color){r_val, g_val, b_val, a};
        
        // GROWING SCALE: 0.4 ke 1.0 berdasarkan opacity
        float growth = 0.4f + (0.6f * mainClouds[i].opacity);
        DrawBakedObject(texCloud, mainClouds[i].x, mainClouds[i].y, growth, cloudTint);
    }

    UpdateBirds(dt);
    DrawBirds();

    // INTERACTION & EDUCATIONAL LABELS
    Vector2 m = GetMousePosition();
    float mouseX = (m.x - (float)G_OriginX) / G_TickStep;
    float mouseY = ((float)G_OriginY - m.y) / G_TickStep;

    if (currentPhase == PHASE_EVAP_TRANSP) {
        if (mouseX > 100 && mouseY < 0) DrawPhaseLabel("Proses Evaporasi");
        else if (mouseX < 0 && mouseY > landTop - 150) DrawPhaseLabel("Proses Transpirasi");
    } 
    else if (currentPhase == PHASE_CONDENSATION) {
        // Deteksi hover pada awan saat menebal
        for (int i = 0; i < 2; i++) {
            float sw = 250.0f * (0.4f + (0.6f * mainClouds[i].opacity));
            float sh = 150.0f * (0.4f + (0.6f * mainClouds[i].opacity));
            Rectangle cloudArea = { CS_Xf(mainClouds[i].x) - sw/2, CS_Yf(mainClouds[i].y) - sh/2, sw, sh };
            if (CheckCollisionPointRec(m, cloudArea)) {
                DrawPhaseLabel("Proses Kondensasi (Pembentukan Awan)");
                break;
            }
        }
    }
    else if (currentPhase == PHASE_ADVECTION) {
        DrawPhaseLabel("Proses Adveksi");
    }
    else if (currentPhase == PHASE_PRECIPITATION) {
        DrawPhaseLabel("Proses Presipitasi");
    }
    else if (currentPhase == PHASE_INFILTR_COLLECT) {
        if (mouseY < landTop && mouseX < 0) {
            if (mouseX > -200 && mouseX < 50) DrawPhaseLabel("Proses Runoff");
            else DrawPhaseLabel("Proses Infiltrasi");
        }
    }

    if (DrawButton(&btnBackSim)) {
        UnloadSimulationTextures();
        currentScreen = MENU_SCREEN;
    }
}