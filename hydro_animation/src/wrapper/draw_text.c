#include "draw_text.h"
#include <math.h>

void Wrapper_DrawText(const char *text, int x, int y, int fontSize, Color color) {
    DrawText(text, x, y, fontSize, color);
}

void Wrapper_DrawTextPremium(const char *text, int x, int y, int fontSize, Color baseColor, Color glowColor) {
    // 1. Shadow (Bawah Kanan)
    DrawText(text, x + 2, y + 2, fontSize, (Color){0, 0, 0, 100});
    
    // 2. Glow (Pancaran tipis di sekeliling)
    Color faintGlow = glowColor;
    faintGlow.a = 50;
    DrawText(text, x - 1, y, fontSize, faintGlow);
    DrawText(text, x + 1, y, fontSize, faintGlow);
    DrawText(text, x, y - 1, fontSize, faintGlow);
    DrawText(text, x, y + 1, fontSize, faintGlow);
    
    // 3. Teks Utama
    DrawText(text, x, y, fontSize, baseColor);
}

void Wrapper_DrawTextFloating(const char *text, int centerX, int baseY, int fontSize, Color color, float time, float amplitude) {
    int textWidth = MeasureText(text, fontSize);
    int x = centerX - textWidth / 2;
    // Osilasi sinus pada Y
    int y = baseY + (int)(sinf(time * 2.0f) * amplitude);
    
    // Gunakan Premium Draw untuk judul floating agar lebih hidup
    Wrapper_DrawTextPremium(text, x, y, fontSize, color, RAYWHITE);
}
