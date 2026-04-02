#include "sun.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_line.h"
#include <math.h>
#include "raylib.h"

void DrawSun(int cx, int cy, int r) {

    Color sunColor = YELLOW;
    float time = GetTime();

    // Denyut ukuran glow menggunakan sin berdasarkan waktu
    float pulse = sin(time * 2.0f); // osilasi -1.0 hingga 1.0
    int glowOffset = (int)(pulse * 5.0f); 

    // =========================
    // INTI MATAHARI DENGAN EFEK GLOW MEMBESAR-MENGECIL
    // =========================
    // Lapis pinggir (paling pudar & besar, dipengaruhi denyut kuat)
    Wrapper_DrawCircleFilled(cx, cy, r + 20 + glowOffset * 2, (Color){255, 255, 0, 30});
    Wrapper_DrawCircleFilled(cx, cy, r + 10 + glowOffset, (Color){255, 255, 0, 80});
    // Lapis tengah (stabil)
    Wrapper_DrawCircleFilled(cx, cy, r, sunColor);
    // Lapis inti (paling terang/panas)
    Wrapper_DrawCircleFilled(cx, cy, r - 15, (Color){255, 255, 200, 255});

    // =========================
    // SINAR MATAHARI (Osilasi memanjang-memendek)
    // =========================
    // Sinar ikut memendek dan memanjang
    int rayLength = r + 30 + (int)(pulse * 8.0f);

    for (int i = 0; i < 360; i += 20) {

        float rad = i * (3.14159f / 180.0f);
        
        // Mempertahankan variasi sinar panjang/pendek
        int currentRayLen = rayLength;
        if (i % 40 == 0) {
            currentRayLen += 15; 
        }

        int x2 = cx + cos(rad) * currentRayLen;
        int y2 = cy + sin(rad) * currentRayLen;

        int x1 = cx + cos(rad) * (r + 10);
        int y1 = cy + sin(rad) * (r + 10);

        Wrapper_DrawLineThick(
            x1, y1,
            x2, y2,
            2,
            (Color){255, 230, 0, 180}
        );
    }
}
