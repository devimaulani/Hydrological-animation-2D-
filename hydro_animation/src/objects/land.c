#include "land.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

void DrawLand() {

    for (int y = -HALF_H / 4.2; y > BOTTOM; y--) {

        Wrapper_DrawLineThick(
            LEFT, y,
            0,    y,
            2,
            (Color){80, 170, 80, 255}
        );
    }
}