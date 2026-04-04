#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Rectangle bounds;
    const char *text;
    Color baseColor;
    Color hoverColor;
    Color textColor;
    bool isHovered;
} Button;

Button CreateButton(float x, float y, float width, float height, const char *text);
bool DrawButton(Button *b);

#endif
