#ifndef SLIDER_H
#define SLIDER_H

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

#endif