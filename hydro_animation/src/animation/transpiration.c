#include "transpiration.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include "../simulation/simulation_state.h"
#include <math.h>

#define BASE_Y  (-HALF_H / 3)
#define CLOUD_Y (TOP - 200)

#define COUNT 4
#define X_START (LEFT + 630)
#define X_GAP   25

#define LINE_THICK 2
#define BASE_SPEED 40.0f

typedef struct {
    float x;
    float y;
} Transp;

static Transp t[COUNT];

void InitTranspiration() {
    for (int i = 0; i < COUNT; i++) {
        t[i].x = X_START + i * X_GAP;
        t[i].y = BASE_Y;
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

static void DrawWave(int baseX, float startY) {

    Color color = (Color){200,255,200,180};

    float amplitude = 5.0f;
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

void DrawTranspiration() {
    for (int i = 0; i < COUNT; i++) {
        DrawWave((int)t[i].x, t[i].y);
    }
}