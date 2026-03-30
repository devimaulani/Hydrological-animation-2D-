#include "draw_line.h"
#include "../algo/bresenham.h"
#include "coords.h"

void Wrapper_DrawLine(int x1, int y1, int x2, int y2, Color color) {

    BresenhamLine(
        CS_X(x1),
        CS_Y(y1),
        CS_X(x2),
        CS_Y(y2),
        color
    );
}

void Wrapper_DrawLineThick(int x1, int y1, int x2, int y2, int thick, Color color) {

    Bres_ThickLine(
        CS_X(x1),
        CS_Y(y1),
        CS_X(x2),
        CS_Y(y2),
        thick * G_TickStep, // 🔥 scaling tebal
        color
    );
}