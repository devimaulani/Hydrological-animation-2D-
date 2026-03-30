#include "sky.h"
#include "raylib.h"

void DrawSky(int isDay) {
    if (isDay)
        ClearBackground(SKYBLUE);
    else
        ClearBackground(DARKBLUE);
}