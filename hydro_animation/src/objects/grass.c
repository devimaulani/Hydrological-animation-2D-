#include "grass.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

void DrawGrassTuft(int cx, int cy, float sway) {
    Color green = {34, 139, 34, 255}; 
    // Tiga helai rumput dengan ujung yang bergoyang (sway)
    Wrapper_DrawLineThick(cx, cy, cx - 12 + sway, cy + 22, 2, green);
    Wrapper_DrawLineThick(cx, cy, cx + sway,      cy + 30, 2, green);
    Wrapper_DrawLineThick(cx, cy, cx + 12 + sway, cy + 20, 2, green);
}
