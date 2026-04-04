#include "draw_circle.h"
#include "../algo/midcircle.h"
#include "coords.h"

void Wrapper_DrawCircle(float x, float y, float r, Color color) {
    Midcircle(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(r * G_TickStep),
        color
    );
}

void Wrapper_DrawCircleFilled(float x, float y, float r, Color color) {
    MidcircleFilled(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(r * G_TickStep),
        color
    );
}