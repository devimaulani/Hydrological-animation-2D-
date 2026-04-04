#include "draw_ellipse.h"
#include "../algo/midellipse.h"
#include "coords.h"

void Wrapper_DrawEllipse(float x, float y, float rx, float ry, Color color) {
    MidpointEllipse(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(rx * G_TickStep),
        (int)(ry * G_TickStep),
        color
    );
}

void Wrapper_DrawEllipseFilled(float x, float y, float rx, float ry, Color color) {
    MidpointEllipseFilled(
        (int)CS_Xf(x),
        (int)CS_Yf(y),
        (int)(rx * G_TickStep),
        (int)(ry * G_TickStep),
        color
    );
}