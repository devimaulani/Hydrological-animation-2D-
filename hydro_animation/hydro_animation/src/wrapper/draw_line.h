#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include "raylib.h"

void Wrapper_DrawLine(int x1, int y1, int x2, int y2, Color color);
void Wrapper_DrawLineThick(int x1, int y1, int x2, int y2, int thick, Color color);

#endif