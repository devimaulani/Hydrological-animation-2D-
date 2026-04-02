#include "grass.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

void DrawGrassTuft(int cx, int cy) {
    Color green = {34, 139, 34, 255}; // Hijau tua untuk detail tekstur
    // Tiga helai sirip rumput zigzag menembus tanah ke atas
    Wrapper_DrawLineThick(cx, cy, cx - 12, cy + 22, 2, green);
    Wrapper_DrawLineThick(cx, cy, cx,      cy + 30, 2, green);
    Wrapper_DrawLineThick(cx, cy, cx + 12, cy + 20, 2, green);
}
