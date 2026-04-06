#include "draw_line.h"
#include "../algo/bresenham.h"
#include "coords.h"
#include "../simulation/simulation_state.h"

void Wrapper_DrawLine(float x1, float y1, float x2, float y2, Color color) {
    if (isWireframeMode && applyWireframeColor) { color.r = 0; color.g = 255; color.b = 0; }
    // Cast ke int hanya di langkah terakhir sebelum memanggil algoritma garis
    BresenhamLine(
        (int)CS_Xf(x1),
        (int)CS_Yf(y1),
        (int)CS_Xf(x2),
        (int)CS_Yf(y2),
        color
    );
}

void Wrapper_DrawLineThick(float x1, float y1, float x2, float y2, float thick, Color color) {
    if (isWireframeMode && applyWireframeColor) { color.r = 0; color.g = 255; color.b = 0; }
    // Scaling tebal tetap menggunakan tick step
    Bres_ThickLine(
        (int)CS_Xf(x1),
        (int)CS_Yf(y1),
        (int)CS_Xf(x2),
        (int)CS_Yf(y2),
        (int)(thick * G_TickStep),
        color
    );
}