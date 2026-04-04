#include "button.h"
#include "../../src/wrapper/draw_rectangle.h"
#include "../../src/wrapper/draw_text.h"

Button CreateButton(float x, float y, float width, float height, const char *text) {
    Button b;
    b.bounds = (Rectangle){ x, y, width, height };
    b.text = text;
    b.baseColor = (Color){ 45, 95, 150, 240 };  // Deep Blue Glass
    b.hoverColor = (Color){ 65, 145, 225, 255 }; // Electric Blue
    b.textColor = RAYWHITE;
    b.isHovered = false;
    return b;
}

bool DrawButton(Button *b) {
    Vector2 mousePos = GetMousePosition();
    b->isHovered = CheckCollisionPointRec(mousePos, b->bounds);
    
    Color currentDrawColor = b->isHovered ? b->hoverColor : b->baseColor;
    int shadowOffset = b->isHovered ? 2 : 4;
    
    // 1. Shadow (Rounded)
    Wrapper_DrawRectangleRounded(
        (int)b->bounds.x + shadowOffset, (int)b->bounds.y + shadowOffset, 
        (int)b->bounds.width, (int)b->bounds.height, 10, (Color){ 0, 0, 0, 40 });
    
    // 2. Main Button Body (Rounded)
    Wrapper_DrawRectangleRounded(
        (int)b->bounds.x, (int)b->bounds.y, 
        (int)b->bounds.width, (int)b->bounds.height, 10, currentDrawColor);
    
    // 3. Border / Outline (Using lines)
    Color borderCol = b->isHovered ? RAYWHITE : (Color){200, 200, 200, 180};
    Wrapper_DrawRectangleOutline(
        (int)b->bounds.x, (int)b->bounds.y, 
        (int)b->bounds.width, (int)b->bounds.height, 2, borderCol);
    
    // 4. Text (Centered)
    int fontSize = 26;
    int textWidth = MeasureText(b->text, fontSize);
    Wrapper_DrawText(b->text, 
             (int)(b->bounds.x + b->bounds.width/2 - textWidth/2), 
             (int)(b->bounds.y + b->bounds.height/2 - fontSize/2), 
             fontSize, b->textColor);
             
    return b->isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
