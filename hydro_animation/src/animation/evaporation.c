#include "evaporation.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include "../simulation/simulation_state.h"
#include <math.h>

#define SEA_Y   (-HALF_H / 3)
#define CLOUD_Y (TOP - 200)

#define VAPOR_COUNT 8
#define X_START (0 + 50)
#define X_GAP   50

#define LINE_THICK 2
#define BASE_SPEED 40.0f

typedef struct {
    float x;
    float y;
} Vapor;

static Vapor vapors[VAPOR_COUNT];

void InitEvaporation() {
    for (int i = 0; i < VAPOR_COUNT; i++) {
        vapors[i].x = X_START + i * X_GAP;
        if (vapors[i].x > RIGHT - 50) vapors[i].x = RIGHT - 50;
        vapors[i].y = SEA_Y;
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

static void DrawWave(int baseX, float startY) {

    Color color = (Color){220,235,255,200};

    float amplitude = 6.0f;
    float frequency = 0.08f;
    float step = 2.0f;

    for (float y = startY; y < CLOUD_Y; y += step) {

        float offset = amplitude * sinf(y * frequency);
        int x = (int)(baseX + offset);

        for (int t = -LINE_THICK/2; t <= LINE_THICK/2; t++) {
            Wrapper_DrawLine(x+t, (int)y, x+t, (int)(y+step), color);
        }
    }
}

void DrawEvaporation() {
    for (int i = 0; i < VAPOR_COUNT; i++) {
        DrawWave((int)vapors[i].x, vapors[i].y);
    }
}