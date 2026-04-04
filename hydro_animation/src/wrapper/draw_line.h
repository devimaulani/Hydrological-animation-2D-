#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include "raylib.h"

// Menggunakan float untuk koordinat agar pergerakan halus (tidak patah-patah)
void Wrapper_DrawLine(float x1, float y1, float x2, float y2, Color color);
void Wrapper_DrawLineThick(float x1, float y1, float x2, float y2, float thick, Color color);

#endif