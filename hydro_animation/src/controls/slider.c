#include "slider.h"
#include "raylib.h"
#include <stdio.h>
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include "coords.h"

void InitSlider(Slider* s, float x, float y, float width, float min, float max, float start) {
    s->x = x;
    s->y = y;
    s->width = width;
    s->minValue = min;
    s->maxValue = max;
    s->value = start;
    s->isDragging = 0;
}

// ===========================
// PERBAIKAN LOGIKA KOORDINAT UNTUK UI
// ===========================
void UpdateSlider(Slider* s) {

    // Hitung posisi knob di layar (bukan cartesian ya, karena mouse pos itu resolusi lokal)
    float knobCartesianX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;
    
    // Titik tombol digeser ke kooridnates layar untuk deteksi sentuh akurat
    int screenKnobX = CS_X((int)knobCartesianX);
    int screenKnobY = CS_Y((int)s->y);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Kotak tumbukan tombol disempurnakan (area 20x20 pixel di sekitar pusat lingkaran)
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){screenKnobX-10, screenKnobY-10, 20, 20})) {
            s->isDragging = 1;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        s->isDragging = 0;
    }

    if (s->isDragging) {
        float mouseX = GetMouseX();

        // Batas rel pada titik layarnya
        float screenMinX = CS_X((int)s->x);
        float screenMaxX = CS_X((int)(s->x + s->width));

        if (mouseX < screenMinX) mouseX = screenMinX;
        if (mouseX > screenMaxX) mouseX = screenMaxX;

        // Proporsi nilai berdasarkan geseran murni layar
        float t = (mouseX - screenMinX) / (screenMaxX - screenMinX);
        s->value = s->minValue + t * (s->maxValue - s->minValue);
    }
}


void DrawSlider(Slider* s, const char* label) {

    float knobX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;

    // garis slider
    Wrapper_DrawLineThick((int)s->x, (int)s->y, (int)(s->x + s->width), (int)s->y, 3, WHITE);

    // knob
    Wrapper_DrawCircleFilled((int)knobX, (int)s->y, 6, YELLOW);

    // label
    char text[64];
    sprintf(text, "%s: %.2f", label, s->value);
    // DrawText menggunakan fungsi asli raylib yang koordinat nol-nya di pojok,
    // maka kita harus konversi titik Cartesian ini ke titik layar.
    DrawText(text, CS_X((int)s->x), CS_Y((int)s->y) - 25, 16, WHITE);
}