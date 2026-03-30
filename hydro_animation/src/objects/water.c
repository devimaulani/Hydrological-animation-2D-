#include "water.h"
#include "coords.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include <math.h>

void DrawWater(int yStart) {

    // ======================
    // BASE WATER
    // ======================
    for (int y = yStart; y > BOTTOM; y--) {

        Wrapper_DrawLineThick(
            0, y,
            RIGHT, y,
            2,
            (Color){30, 144, 255, 255}
        );
    }

    // ======================
    // GELOMBANG
    // ======================
    for (int x = 50; x < RIGHT - 50; x += 50) {

        int cx = x;
        int cy = yStart + 10;

        int rx = 15;
        int ry = 5;

        for (int i = 0; i <= 180; i += 10) {

            float rad = i * (3.14159f / 180.0f);

            int px = cx + rx * cos(rad);
            int py = cy + ry * sin(rad);

            Wrapper_DrawLine(px, py, px, py, (Color){200,220,255,255});
        }
    }
}