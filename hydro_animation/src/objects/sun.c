#include "sun.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_line.h"
#include <math.h>
#include "raylib.h"

void DrawSun(float cx, float cy, float r, float opacity) {
    if (opacity <= 0.0f) return;

    float time = (float)GetTime();

    float pulse = sinf(time * 1.5f); 
    
    unsigned char baseAlpha = (unsigned char)(255 * opacity);
    
    //pancaran cahaya
    for (int i = 0; i < 3; i++) {
        // Fase waktu yang berbeda untuk tiap cincin (0.0, 0.33, 0.66)
        float offset = i * 0.33f;
        // Progress siklus (0.0 = pusat, 1.0 = hilang di kejauhan)
        float progress = fmodf(time * 0.5f + offset, 1.0f);
        
        // Ukuran ring memancar dari inti hingga sangat lebar
        float ringRadius = r + (progress * 130.0f);
        // Alpha memudar seiring ukuran membesar
        unsigned char ringAlpha = (unsigned char)(100 * (1.0f - progress) * opacity);
        
        if (ringAlpha > 5) {
            Color ringColor = (Color){255, 255, 150, ringAlpha};
            Wrapper_DrawCircleFilled(cx, cy, ringRadius, ringColor);
        }
    }

    // Lapis utama - Satu warna Kuning Bersinar yang Solid
    Wrapper_DrawCircleFilled(cx, cy, r, (Color){255, 235, 0, baseAlpha});
}