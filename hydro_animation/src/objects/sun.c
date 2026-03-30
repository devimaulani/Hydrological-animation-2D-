#include "sun.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_line.h"
#include <math.h>

void DrawSun(int cx, int cy, int r) {

    Color sunColor = YELLOW;

    // =========================
    // INTI MATAHARI
    // =========================
    Wrapper_DrawCircleFilled(cx, cy, r, sunColor);

    // =========================
    // SINAR MATAHARI
    // =========================
    int rayLength = r + 20;

    for (int i = 0; i < 360; i += 30) {

        float rad = i * (3.14159f / 180.0f);

        int x2 = cx + cos(rad) * rayLength;
        int y2 = cy + sin(rad) * rayLength; // ✔ sudah sesuai Cartesian

        Wrapper_DrawLineThick(
            cx, cy,
            x2, y2,
            2,   // 🔥 biar solid & terlihat jelas
            sunColor
        );
    }
}