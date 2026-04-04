#include "transpiration.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include "../simulation/simulation_state.h"
#include <math.h>

#define BASE_Y  (-HALF_H / 4)
#define CLOUD_Y (TOP - 200)

#define COUNT 3
#define X_START (LEFT + 670) // Difokuskan hanya di kerumunan 3 pohon paling kanan (zona pesisir)
#define X_GAP   45           // Jarak dirapatkan antar pohon kanan

#define BASE_SPEED 45.0f

// Zona memudar: uap mulai memudar di 70% perjalanan ke awan
#define FADE_START_RATIO 0.7f

typedef struct {
    float x;
    float y;
    float phase; // Fase dinamis asimetris
} Transp;

static Transp t[COUNT];

void InitTranspiration() {
    for (int i = 0; i < COUNT; i++) {
        t[i].x = X_START + i * X_GAP;
        t[i].y = GetRandomValue(BASE_Y, CLOUD_Y); // Acak elevasi agar natural
        t[i].phase = GetRandomValue(0, 314) / 100.0f;
    }
}

void UpdateTranspiration(float dt) {
    for (int i = 0; i < COUNT; i++) {

        t[i].y += BASE_SPEED * transpirationRate * sunIntensity * dt;

        if (t[i].y > CLOUD_Y) {
            t[i].y = BASE_Y;
        }
    }
}

static void DrawWave(float xBase, float yBase, float phase) {
    Color color = (Color){200, 255, 200, 180}; // Hijau keputihan untuk uap daun
    float amplitude = 6.0f;
    float frequency = 0.05f;
    float step = 3.0f;
    float length = 70.0f;

    // Hitung progress perjalanan (0.0 = di tanah, 1.0 = di awan)
    float totalRange = (float)(CLOUD_Y - BASE_Y);
    float progress = (yBase - (float)BASE_Y) / totalRange;
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
        unsigned char alpha = (unsigned char)(180 * alphaFactor * totalOpacity);
        if (alpha < 3) continue;  // Lewati jika terlalu transparan
        
        float currentY = yBase + dy;
        float waktu = (float)GetTime() * 1.5f;
        
        float offset = amplitude * sinf(currentY * frequency + phase + waktu);
        float nextOffset = amplitude * sinf((currentY + step) * frequency + phase + waktu);
        
        Color renderColor = (Color){color.r, color.g, color.b, alpha};
        Wrapper_DrawLineThick(xBase + offset, currentY, xBase + nextOffset, currentY + step, 3.0f, renderColor);
    }
}

void DrawTranspiration() {
    for (int i = 0; i < COUNT; i++) {
        DrawWave(t[i].x, t[i].y, t[i].phase);
    }
}