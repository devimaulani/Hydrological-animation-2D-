#include "draw_circle.h"
#include "../algo/midcircle.h"
#include "coords.h"
#include "../simulation/simulation_state.h"

void Wrapper_DrawCircle(float x, float y, float r, Color color) {
    if (isWireframeMode && applyWireframeColor) { color.r = 0; color.g = 255; color.b = 0; }
    Midcircle(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(r * G_TickStep),
        color
    );
}

void Wrapper_DrawCircleFilled(float x, float y, float r, Color color) {
    if (isWireframeMode) {
        Wrapper_DrawCircle(x, y, r, color);
        return;
    }
    MidcircleFilled(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(r * G_TickStep),
        color
    );
}