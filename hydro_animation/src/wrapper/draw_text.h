#ifndef DRAW_TEXT_H
#define DRAW_TEXT_H

#include "raylib.h"

// Menggambar teks standar melalui wrapper
void Wrapper_DrawText(const char *text, int x, int y, int fontSize, Color color);

// Menggambar teks dengan efek bayangan dan glow (Premium)
void Wrapper_DrawTextPremium(const char *text, int x, int y, int fontSize, Color baseColor, Color glowColor);

// Menggambar teks yang mengambang (animasi sinus)
void Wrapper_DrawTextFloating(const char *text, int centerX, int baseY, int fontSize, Color color, float time, float amplitude);

#endif
