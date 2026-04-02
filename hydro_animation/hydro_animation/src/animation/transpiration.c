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

    for (float dy = 0; dy < length; dy += step) {
        // Efek transparan/fade di pangkal dan ujung
        float alphaFactor = 1.0f - fabsf(dy - (length / 2.0f)) / (length / 2.0f);
        if (alphaFactor < 0.0f) alphaFactor = 0.0f;
        unsigned char alpha = (unsigned char)(180 * alphaFactor);
        
        float currentY = yBase + dy;
        float waktu = GetTime() * 1.5f;
        
        // Gelombang bergerak dan mendayu sesuai waktu
        float offset = amplitude * sinf(currentY * frequency + phase + waktu);
        float nextOffset = amplitude * sinf((currentY + step) * frequency + phase + waktu);
        
        Color renderColor = (Color){color.r, color.g, color.b, alpha};
        Wrapper_DrawLineThick((int)(xBase + offset), (int)currentY, (int)(xBase + nextOffset), (int)(currentY + step), 3, renderColor);
    }
}

void DrawTranspiration() {
    for (int i = 0; i < COUNT; i++) {
        DrawWave(t[i].x, t[i].y, t[i].phase);
    }
}