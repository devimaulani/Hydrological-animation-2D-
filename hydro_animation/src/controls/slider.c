#include "slider.h"
#include "raylib.h"
#include <stdio.h>

void InitSlider(Slider* s, float x, float y, float width, float min, float max, float start) {
    s->x = x;
    s->y = y;
    s->width = width;
    s->minValue = min;
    s->maxValue = max;
    s->value = start;
    s->isDragging = 0;
}

void UpdateSlider(Slider* s) {

    float knobX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){knobX-5, s->y-5, 10, 10})) {
            s->isDragging = 1;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        s->isDragging = 0;
    }

    if (s->isDragging) {
        float mouseX = GetMouseX();

        if (mouseX < s->x) mouseX = s->x;
        if (mouseX > s->x + s->width) mouseX = s->x + s->width;

        float t = (mouseX - s->x) / s->width;
        s->value = s->minValue + t * (s->maxValue - s->minValue);
    }
}

void DrawSlider(Slider* s, const char* label) {

    float knobX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;

    // garis slider
    DrawLine(s->x, s->y, s->x + s->width, s->y, WHITE);

    // knob
    DrawCircle(knobX, s->y, 6, YELLOW);

    // label
    char text[64];
    sprintf(text, "%s: %.2f", label, s->value);
    DrawText(text, s->x, s->y - 25, 16, WHITE);
}