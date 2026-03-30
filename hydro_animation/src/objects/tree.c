#include "tree.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

// ==========================
// BATANG
// ==========================
void DrawTrunk(int x, int y) {

    int height = 40;

    Wrapper_DrawLineThick(
        x, y,
        x, y + height,   // 🔥 ke atas (positif)
        3,
        BROWN
    );
}

// ==========================
// DAUN
// ==========================
void DrawLeaves(int x, int y) {

    int offset = 50;

    Wrapper_DrawCircleFilled(x, y + offset + 10, 18, GREEN);
    Wrapper_DrawCircleFilled(x - 15, y + offset, 15, GREEN);
    Wrapper_DrawCircleFilled(x + 15, y + offset, 15, GREEN);
    Wrapper_DrawCircleFilled(x, y + offset - 10, 15, GREEN);
}

// ==========================
// POHON
// ==========================
void DrawTree(int x, int y) {
    DrawTrunk(x, y);
    DrawLeaves(x, y);
}