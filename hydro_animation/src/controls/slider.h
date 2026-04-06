#ifndef SLIDER_H
#define SLIDER_H

#include "raylib.h"
#include <stdbool.h>

/* -----------------------------------------------------------------------
 * Slider lama (koordinat Cartesian) — dipertahankan agar tidak break
 * ----------------------------------------------------------------------- */
typedef struct {
    float x, y;
    float width;
    float minValue;
    float maxValue;
    float value;
    int isDragging;
} Slider;

void InitSlider(Slider* s, float x, float y, float width, float min, float max, float start);
void UpdateSlider(Slider* s);
void DrawSlider(Slider* s, const char* label);

/* -----------------------------------------------------------------------
 * SliderScreen — slider modern berbasis koordinat PIXEL LAYAR langsung.
 * Cocok untuk UI pojok layar (seperti Button yang sudah ada).
 *
 * Fitur:
 *  - Panel glass semi-transparan
 *  - Track bergradasi (gelap → terang)
 *  - Knob bulat dengan highlight
 *  - Label kecepatan: "Lambat / Normal / Cepat / Sangat Cepat"
 *  - Animasi hover knob
 * ----------------------------------------------------------------------- */
typedef struct {
    float x;          /* Pojok kiri-atas panel (pixel) */
    float y;          /* Pojok kiri-atas panel (pixel) */
    float panelW;     /* Lebar panel */
    float panelH;     /* Tinggi panel */
    float minValue;
    float maxValue;
    float value;
    bool  isDragging;
    bool  isHovered;  /* Hover pada knob */
} SliderScreen;

void InitSliderScreen(SliderScreen* s,
                      float x, float y,
                      float panelW, float panelH,
                      float min, float max, float start);

void UpdateSliderScreen(SliderScreen* s);
void DrawSliderScreen(SliderScreen* s, const char* title);

#endif /* SLIDER_H */