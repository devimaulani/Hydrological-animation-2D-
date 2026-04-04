#include "evaporation.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include "../simulation/simulation_state.h"
#include <math.h>

#define SEA_Y   (-HALF_H / 3 - 50)
#define CLOUD_Y (TOP - 200)

#define VAPOR_COUNT 8
#define X_START 200     // Bergeser ke kanan karena daerah daratan / tebing diperlebar ke area X=100
#define X_GAP   70      // Sebarkan merata ke seluruh lautan

#define BASE_SPEED 50.0f

// Zona memudar: uap mulai memudar di 80% perjalanan ke awan
#define FADE_START_RATIO 0.7f

typedef struct {
    float x;
    float y;
    float phase; // Fase agar animasi meliuk tiap helai berbeda
} Vapor;

static Vapor vapors[VAPOR_COUNT];

void InitEvaporation() {
    for (int i = 0; i < VAPOR_COUNT; i++) {
        vapors[i].x = X_START + i * X_GAP;
        if (vapors[i].x > RIGHT - 50) vapors[i].x = RIGHT - 50;
        vapors[i].y = GetRandomValue(SEA_Y, CLOUD_Y); // Acak posisi awal agar tidak jalan bergerombol
        vapors[i].phase = GetRandomValue(0, 314) / 100.0f;
    }
}

void UpdateEvaporation(float dt) {
    for (int i = 0; i < VAPOR_COUNT; i++) {

        // dipengaruhi matahari
        vapors[i].y += BASE_SPEED * evaporationRate * sunIntensity * dt;

        if (vapors[i].y > CLOUD_Y) {
            vapors[i].y = SEA_Y;
        }
    }
}

static void DrawWave(float xBase, float yBase, float phase) {
    Color color = (Color){220, 240, 255, 180}; // Warna uap transparan
    float amplitude = 8.0f;
    float frequency = 0.04f;
    float step = 3.0f;
    float length = 80.0f; // Helai uap air pendek dan terbatas

    // Hitung progress perjalanan (0.0 = di laut, 1.0 = di awan)
    float totalRange = (float)(CLOUD_Y - SEA_Y);
    float progress = (yBase - (float)SEA_Y) / totalRange;
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    
    // Faktor memudar: mulai memudar setelah 70% perjalanan
    float fadeFactor = 1.0f;
    if (progress > FADE_START_RATIO) {
        // Memudar secara bertahap dari 1.0 ke 0.0
        fadeFactor = 1.0f - ((progress - FADE_START_RATIO) / (1.0f - FADE_START_RATIO));
        if (fadeFactor < 0.0f) fadeFactor = 0.0f;
    }
    
    // Juga memudar di awal perjalanan (muncul secara bertahap)
    float appearFactor = 1.0f;
    if (progress < 0.15f) {
        appearFactor = progress / 0.15f;
    }
    
    float totalOpacity = fadeFactor * appearFactor;

    for (float dy = 0; dy < length; dy += step) {
        float alphaFactor = 1.0f - fabsf(dy - (length / 2.0f)) / (length / 2.0f);
        if (alphaFactor < 0.0f) alphaFactor = 0.0f;
        unsigned char alpha = (unsigned char)(200 * alphaFactor * totalOpacity);
        if (alpha < 3) continue;  // Lewati jika terlalu transparan
        
        float currentY = yBase + dy;
        float t = (float)GetTime() * 2.0f;
        
        float offset = amplitude * sinf(currentY * frequency + phase + t);
        float nextOffset = amplitude * sinf((currentY + step) * frequency + phase + t);
        
        Color renderColor = (Color){color.r, color.g, color.b, alpha};
        Wrapper_DrawLineThick(xBase + offset, currentY, xBase + nextOffset, currentY + step, 3.0f, renderColor);
    }
}

void DrawEvaporation() {
    for (int i = 0; i < VAPOR_COUNT; i++) {
        DrawWave(vapors[i].x, vapors[i].y, vapors[i].phase);
    }
}