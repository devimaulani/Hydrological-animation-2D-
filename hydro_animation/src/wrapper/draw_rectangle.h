#ifndef DRAW_RECTANGLE_H
#define DRAW_RECTANGLE_H

#include "raylib.h"

// Menggambar garis tepi persegi panjang menggunakan algoritma kustom
void Wrapper_DrawRectangleOutline(int x, int y, int width, int height, int thickness, Color color);

// Menggambar persegi panjang berisi (solid) menggunakan scanline / horizontal lines
void Wrapper_DrawRectangleFilled(int x, int y, int width, int height, Color color);

// Menggambar persegi panjang dengan sudut membulat (Rounded)
void Wrapper_DrawRectangleRounded(int x, int y, int width, int height, int radius, Color color);

#endif
