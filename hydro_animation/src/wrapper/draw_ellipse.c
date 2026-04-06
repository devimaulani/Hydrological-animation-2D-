#include "draw_ellipse.h"
#include "../algo/midellipse.h"
#include "coords.h"
#include "../simulation/simulation_state.h"

void Wrapper_DrawEllipse(float x, float y, float rx, float ry, Color color) {
    if (isWireframeMode && applyWireframeColor) { color.r = 0; color.g = 255; color.b = 0; }
    MidpointEllipse(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(rx * G_TickStep),
        (int)(ry * G_TickStep),
        color
    );
}

void Wrapper_DrawEllipseFilled(float x, float y, float rx, float ry, Color color) {
    if (isWireframeMode) {
        Wrapper_DrawEllipse(x, y, rx, ry, color);
        return;
    }
    MidpointEllipseFilled(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(rx * G_TickStep),
        (int)(ry * G_TickStep),
        color
    );
}