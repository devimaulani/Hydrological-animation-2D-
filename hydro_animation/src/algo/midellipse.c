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
    if (rx <= 0 || ry <= 0) return;

    int x = 0;
    int y = ry;

    float d1 = (ry * ry) - (rx * rx * ry) + (0.25f * rx * rx);
    long dx = 2 * ry * ry * x;
    long dy = 2 * rx * rx * y;

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

    float d2 = (ry * ry * (x + 0.5f) * (x + 0.5f)) +
               (rx * rx * (y - 1) * (y - 1)) -
               (rx * rx * ry * ry);

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

void MidpointEllipseFilled(int cx, int cy, int rx, int ry, Color color) {
    if (rx <= 0 || ry <= 0) return;

    int x = 0;
    int y = ry;

    float d1 = (ry * ry) - (rx * rx * ry) + (0.25f * rx * rx);
    long dx = 2 * ry * ry * x;
    long dy = 2 * rx * rx * y;

    while (dx < dy) {
        // Draw horizontal lines between points
        for (int i = cx - x; i <= cx + x; i++) {
            DrawPixel(i, cy + y, color);
            DrawPixel(i, cy - y, color);
        }

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

    float d2 = (ry * ry * (x + 0.5f) * (x + 0.5f)) +
               (rx * rx * (y - 1) * (y - 1)) -
               (rx * rx * ry * ry);

    while (y >= 0) {
        for (int i = cx - x; i <= cx + x; i++) {
            DrawPixel(i, cy + y, color);
            DrawPixel(i, cy - y, color);
        }

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