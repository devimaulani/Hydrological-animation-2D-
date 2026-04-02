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
    float length = 80.0f; // Helai uap air pendek dan terbatas, bukan tali bersambung hingga awan

    for (float dy = 0; dy < length; dy += step) {
        // Logika fade-out: transparan di bagian pangkal dan ujung helai, solid di tengah
        float alphaFactor = 1.0f - fabsf(dy - (length / 2.0f)) / (length / 2.0f);
        if (alphaFactor < 0.0f) alphaFactor = 0.0f;
        unsigned char alpha = (unsigned char)(200 * alphaFactor);
        
        float currentY = yBase + dy;
        float t = GetTime() * 2.0f;
        
        // Meliuk kekiri dan kekanan tertiup angin (dinamis dengan waktu)
        float offset = amplitude * sinf(currentY * frequency + phase + t);
        float nextOffset = amplitude * sinf((currentY + step) * frequency + phase + t);
        
        Color renderColor = (Color){color.r, color.g, color.b, alpha};
        Wrapper_DrawLineThick((int)(xBase + offset), (int)currentY, (int)(xBase + nextOffset), (int)(currentY + step), 3, renderColor);
    }
}

void DrawEvaporation() {
    for (int i = 0; i < VAPOR_COUNT; i++) {
        DrawWave(vapors[i].x, vapors[i].y, vapors[i].phase);
    }
}