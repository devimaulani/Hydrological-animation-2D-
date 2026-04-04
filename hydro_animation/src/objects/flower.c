#include "flower.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "coords.h"

void DrawFlower(int cx, int cy, float sway) {
    Color green = {34, 139, 34, 255};
    // Batang bergoyang di ujung
    Wrapper_DrawLineThick(cx, cy, cx + sway, cy + 25, 2, green);
    // Daun kecil ikut miring sedikit
    Wrapper_DrawLineThick(cx + sway*0.4f, cy + 10, cx - 8 + sway*0.4f, cy + 18, 2, green);

    int fy = cy + 25; // Titik kelopaknya ikut bergeser sesuai sway
    float fx = cx + sway;

    // 4 kelopak bunga & inti serbuk bergoyang bersama batang
    Wrapper_DrawEllipseFilled(fx - 6, fy, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(fx + 6, fy, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(fx, fy - 6, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(fx, fy + 6, 6, 6, (Color){255, 182, 193, 255});
    Wrapper_DrawEllipseFilled(fx, fy, 5, 5, (Color){255, 215, 0,   255});
}
