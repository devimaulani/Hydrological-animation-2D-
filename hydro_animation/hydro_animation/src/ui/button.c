#include "button.h"

Button CreateButton(float x, float y, float width, float height, const char *text) {
    Button b;
    b.bounds = (Rectangle){ x, y, width, height };
    b.text = text;
    b.baseColor = (Color){ 60, 120, 180, 255 }; // Steel Blue
    b.hoverColor = (Color){ 80, 160, 220, 255 }; // Lighter Blue
    b.textColor = RAYWHITE;
    b.isHovered = false;
    return b;
}

bool DrawButton(Button *b) {
    Vector2 mousePos = GetMousePosition();
    b->isHovered = CheckCollisionPointRec(mousePos, b->bounds);
    
    Color currentDrawColor = b->isHovered ? b->hoverColor : b->baseColor;
    
    // Shadow
    DrawRectangle(b->bounds.x + 4, b->bounds.y + 4, b->bounds.width, b->bounds.height, (Color){ 0, 0, 0, 50 });
    
    // Main Rectangle
    DrawRectangleRec(b->bounds, currentDrawColor);
    DrawRectangleLinesEx(b->bounds, 2, WHITE);
    
    // Text
    int fontSize = 24;
    int textWidth = MeasureText(b->text, fontSize);
    DrawText(b->text, 
             b->bounds.x + b->bounds.width/2 - textWidth/2, 
             b->bounds.y + b->bounds.height/2 - fontSize/2, 
             fontSize, b->textColor);
             
    return b->isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
