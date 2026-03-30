#include "advection.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>

// =========================
// POSISI
// =========================
#define START_X (RIGHT - 400)
#define START_Y (TOP - 250)

#define LAND_X (LEFT + 200)

#define SPEED 80.0f

// =========================
#define THICK 2

static float windX;

// =========================
void InitAdvection() {
    windX = START_X;
}

// =========================
void UpdateAdvection(float dt) {

    windX -= SPEED * dt;

    if (windX < LEFT) {
        windX = START_X;
    }
}


// =========================
// SPIRAL (LEBIH JELAS)
// =========================
static void DrawSpiral(int cx, int cy) {

    float r = 3;

    float prevX = cx;
    float prevY = cy;

    for (float a = 0; a < 6.28f * 2; a += 0.1f) {

        float x = cx + cos(a) * r;
        float y = cy + sin(a) * r;

        // garis antar titik (ini kunci spiral terlihat)
        Wrapper_DrawLineThick(
            (int)prevX, (int)prevY,
            (int)x, (int)y,
            2,
            WHITE
        );

        prevX = x;
        prevY = y;

        r += 0.2f;
    }
}

// =========================
// 🔥 GELOMBANG ANGIN (SMOOTH)
// =========================
static void DrawWind(int baseX, int baseY, float phase) {

    float amp = 10;
    float freq = 0.05f;

    for (int i = 0; i < 220; i += 2) {

        int x1 = baseX - i;
        int y1 = baseY + amp * sinf(i * freq + phase);

        int x2 = baseX - (i + 2);
        int y2 = baseY + amp * sinf((i + 2) * freq + phase);

        Wrapper_DrawLineThick(x1, y1, x2, y2, 2, WHITE);
    }
}

// =========================
// 🔥 DRAW FINAL (2 ANGIN)
// =========================
void DrawAdvection() {

    int x = (int)windX;

    // =========================
    // ANGIN 1 (ATAS)
    // =========================
    int y1 = START_Y;

    DrawSpiral(x, y1);
    DrawWind(x, y1, 0.0f);

    // =========================
    // ANGIN 2 (BAWAH - TIDAK SEJAJAR)
    // =========================
    int y2 = START_Y - 40; // 🔥 beda posisi (tidak sejajar)

    DrawSpiral(x + 20, y2 + 10);        // sedikit offset X
    DrawWind(x + 20, y2, 2.0f);    // beda phase → tidak identik
}

// =========================
int IsWindReachedLand() {
    return windX <= LAND_X;
}