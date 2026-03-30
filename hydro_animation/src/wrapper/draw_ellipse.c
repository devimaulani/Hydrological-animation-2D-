#include "draw_ellipse.h"
#include "../algo/midellipse.h"
#include "coords.h"
#include <math.h>

void Wrapper_DrawEllipse(int cx, int cy, int rx, int ry, Color color) {

    MidpointEllipse(
        CS_X(cx),
        CS_Y(cy),
        rx * G_TickStep,
        ry * G_TickStep,
        color
    );
}

void Wrapper_DrawEllipseFilled(int cx, int cy, int rx, int ry, Color color) {

    int scaled_rx = rx * G_TickStep;
    int scaled_ry = ry * G_TickStep;

    for (int y = -scaled_ry; y <= scaled_ry; y++) {

        int xLimit = scaled_rx * sqrt(1.0 - (float)(y*y)/(scaled_ry*scaled_ry));

        for (int x = -xLimit; x <= xLimit; x++) {
            DrawPixel(
                CS_X(cx) + x,
                CS_Y(cy) + y,
                color
            );
        }
    }
}