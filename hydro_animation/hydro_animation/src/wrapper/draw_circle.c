#include "draw_circle.h"
#include "../algo/midcircle.h"
#include "coords.h"

void Wrapper_DrawCircle(int x, int y, int r, Color color) {

    Midcircle(
        CS_X(x),
        CS_Y(y),
        r * G_TickStep,
        color
    );
}

void Wrapper_DrawCircleFilled(int x, int y, int r, Color color) {

    MidcircleFilled(
        CS_X(x),
        CS_Y(y),
        r * G_TickStep,
        color
    );
}