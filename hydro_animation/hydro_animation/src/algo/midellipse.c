#include "midellipse.h"
#include <math.h>

// plot 4 titik simetri ellipse
static void PlotEllipsePoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color);
    DrawPixel(cx - x, cy + y, color);
    DrawPixel(cx + x, cy - y, color);
    DrawPixel(cx - x, cy - y, color);
}

void MidpointEllipse(int cx, int cy, int rx, int ry, Color color) {

    int x = 0;
    int y = ry;

    // decision parameter region 1
    float d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);

    int dx = 2 * ry * ry * x;
    int dy = 2 * rx * rx * y;

    // REGION 1
    while (dx < dy) {
        PlotEllipsePoints(cx, cy, x, y, color);

        if (d1 < 0) {
            x++;
            dx += 2 * ry * ry;
            d1 += dx + (ry * ry);
        } else {
            x++; y--;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d1 += dx - dy + (ry * ry);
        }
    }

    // decision parameter region 2
    float d2 = (ry * ry * (x + 0.5) * (x + 0.5)) +
               (rx * rx * (y - 1) * (y - 1)) -
               (rx * rx * ry * ry);

    // REGION 2
    while (y >= 0) {
        PlotEllipsePoints(cx, cy, x, y, color);

        if (d2 > 0) {
            y--;
            dy -= 2 * rx * rx;
            d2 += (rx * rx) - dy;
        } else {
            y--; x++;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d2 += dx - dy + (rx * rx);
        }
    }
}