#include "flower.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "coords.h"

void DrawFlower(int cx, int cy) {
    Color green = {34, 139, 34, 255};
    // Batang dan sehelai daun
    Wrapper_DrawLineThick(cx, cy, cx, cy + 25, 2, green);
    Wrapper_DrawLineThick(cx, cy + 10, cx - 8, cy + 18, 2, green);

    int fy = cy + 25; // Titik kelopaknya
    // 4 kelopak bunga (merah muda) & inti serbuk (kuning emas)
    Wrapper_DrawEllipseFilled(cx - 6, fy, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(cx + 6, fy, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(cx, fy - 6, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(cx, fy + 6, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(cx, fy, 5, 5, (Color){255, 215, 0,   255});
}
