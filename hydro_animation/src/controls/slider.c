#include "slider.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_rectangle.h"
#include "coords.h"

/* =====================================================================
 * Slider lama (koordinat Cartesian) — tidak diubah
 * ===================================================================== */

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
    float knobCartesianX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;
    int screenKnobX = CS_X((int)knobCartesianX);
    int screenKnobY = CS_Y((int)s->y);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(),
            (Rectangle){screenKnobX-10, screenKnobY-10, 20, 20})) {
            s->isDragging = 1;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) s->isDragging = 0;

    if (s->isDragging) {
        float mouseX   = GetMouseX();
        float screenMinX = CS_X((int)s->x);
        float screenMaxX = CS_X((int)(s->x + s->width));
        if (mouseX < screenMinX) mouseX = screenMinX;
        if (mouseX > screenMaxX) mouseX = screenMaxX;
        float t = (mouseX - screenMinX) / (screenMaxX - screenMinX);
        s->value = s->minValue + t * (s->maxValue - s->minValue);
    }
}

void DrawSlider(Slider* s, const char* label) {
    float knobX = s->x + ((s->value - s->minValue) / (s->maxValue - s->minValue)) * s->width;
    Wrapper_DrawLineThick((int)s->x, (int)s->y,
                          (int)(s->x + s->width), (int)s->y, 3, WHITE);
    Wrapper_DrawCircleFilled((int)knobX, (int)s->y, 6, YELLOW);
    char text[64];
    sprintf(text, "%s: %.2f", label, s->value);
    DrawText(text, CS_X((int)s->x), CS_Y((int)s->y) - 25, 16, WHITE);
}

/* =====================================================================
 * SliderScreen — UI modern, koordinat PIXEL LAYAR langsung
 * ===================================================================== */

/* Margin dalam panel */
#define SP_PAD_X    16.0f
#define SP_PAD_TOP  14.0f
#define SP_PAD_BOT  14.0f
#define SP_TRACK_H   4.0f   /* Ketebalan track */
#define SP_KNOB_R   10.0f   /* Radius knob normal */
#define SP_KNOB_R_H 12.0f   /* Radius knob saat hover/drag */

/* Hitung posisi X knob di layar */
static float KnobScreenX(const SliderScreen* s) {
    float trackX0 = s->x + SP_PAD_X;
    float trackW  = s->panelW - SP_PAD_X * 2.0f;
    float t = (s->value - s->minValue) / (s->maxValue - s->minValue);
    return trackX0 + t * trackW;
}

/* Hitung posisi Y track di layar */
static float TrackScreenY(const SliderScreen* s) {
    /* Track berada di tengah-tengah vertikal panel */
    return s->y + s->panelH * 0.62f;
}

void InitSliderScreen(SliderScreen* s,
                      float x, float y,
                      float panelW, float panelH,
                      float min, float max, float start) {
    s->x         = x;
    s->y         = y;
    s->panelW    = panelW;
    s->panelH    = panelH;
    s->minValue  = min;
    s->maxValue  = max;
    s->value     = start;
    s->isDragging = false;
    s->isHovered  = false;
}

void UpdateSliderScreen(SliderScreen* s) {
    float kx = KnobScreenX(s);
    float ky = TrackScreenY(s);

    /* Cek hover pada knob */
    Vector2 mouse = GetMousePosition();
    float dx = mouse.x - kx;
    float dy = mouse.y - ky;
    float dist = sqrtf(dx*dx + dy*dy);
    s->isHovered = (dist <= SP_KNOB_R_H + 4.0f);

    /* Mulai drag */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && s->isHovered) {
        s->isDragging = true;
    }
    /* Lepas drag */
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        s->isDragging = false;
    }

    /* Update nilai saat drag */
    if (s->isDragging) {
        float trackX0 = s->x + SP_PAD_X;
        float trackW  = s->panelW - SP_PAD_X * 2.0f;
        float mx = mouse.x;
        if (mx < trackX0)         mx = trackX0;
        if (mx > trackX0 + trackW) mx = trackX0 + trackW;
        float t = (mx - trackX0) / trackW;
        s->value = s->minValue + t * (s->maxValue - s->minValue);
    }
}

void DrawSliderScreen(SliderScreen* s, const char* title) {
    float px = s->x, py = s->y, pw = s->panelW, ph = s->panelH;
    float trackY = TrackScreenY(s);
    float trackX0 = px + SP_PAD_X;
    float trackW  = pw - SP_PAD_X * 2.0f;
    float kx = KnobScreenX(s);
    float knobR = (s->isHovered || s->isDragging) ? SP_KNOB_R_H : SP_KNOB_R;

    /* --- Panel glass background --- */
    /* Shadow */
    DrawRectangleRounded((Rectangle){px+4, py+4, pw, ph}, 0.25f, 8,
                         (Color){0, 0, 0, 50});
    /* Panel utama */
    DrawRectangleRounded((Rectangle){px, py, pw, ph}, 0.25f, 8,
                         (Color){15, 25, 55, 210});
    /* Border atas (highlight) */
    DrawRectangleRounded((Rectangle){px, py, pw, 2}, 0.0f, 4,
                         (Color){100, 160, 255, 120});
    /* Outline */
    DrawRectangleRoundedLines((Rectangle){px, py, pw, ph}, 0.25f, 8,
                              (Color){70, 120, 200, 160});

    /* --- Judul --- */
    int titleFontSize = 15;
    const char* icon = "  Kecepatan Penguapan";
    int titleW = MeasureText(icon, titleFontSize);
    DrawText(icon, (int)(px + pw/2 - titleW/2), (int)(py + SP_PAD_TOP), titleFontSize,
             (Color){180, 210, 255, 230});

    /* --- Track background (abu gelap) --- */
    DrawRectangleRounded(
        (Rectangle){trackX0, trackY - SP_TRACK_H/2, trackW, SP_TRACK_H},
        1.0f, 4, (Color){60, 70, 100, 200});

    /* --- Track fill (biru → cyan sesuai nilai) --- */
    float fillW = (kx - trackX0);
    if (fillW > 0.0f) {
        DrawRectangleRounded(
            (Rectangle){trackX0, trackY - SP_TRACK_H/2, fillW, SP_TRACK_H},
            1.0f, 4, (Color){80, 180, 255, 240});
    }

    /* --- Knob shadow --- */
    DrawCircleV((Vector2){kx+2, trackY+2}, knobR,
                (Color){0, 0, 0, 80});

    /* --- Knob body --- */
    Color knobColor = s->isDragging
        ? (Color){120, 220, 255, 255}
        : (s->isHovered ? (Color){100, 200, 255, 255} : (Color){80, 160, 240, 255});
    DrawCircleV((Vector2){kx, trackY}, knobR, knobColor);

    /* --- Knob highlight (titik putih kecil di kiri atas knob) --- */
    DrawCircleV((Vector2){kx - knobR*0.3f, trackY - knobR*0.35f},
                knobR * 0.28f, (Color){255, 255, 255, 160});

    /* --- Label kecepatan teks --- */
    const char* speedLabel;
    float t = (s->value - s->minValue) / (s->maxValue - s->minValue);
    if      (t < 0.25f) speedLabel = "Lambat";
    else if (t < 0.50f) speedLabel = "Normal";
    else if (t < 0.75f) speedLabel = "Cepat";
    else                speedLabel = "Sangat Cepat";

    int labelFontSize = 13;
    int labelW = MeasureText(speedLabel, labelFontSize);
    DrawText(speedLabel,
             (int)(px + pw/2 - labelW/2),
             (int)(trackY + knobR + 5),
             labelFontSize,
             (Color){200, 230, 255, 200});

    /* --- Nilai numerik kecil di kanan bawah panel --- */
    char valText[16];
    sprintf(valText, "x%.1f", s->value);
    int valW = MeasureText(valText, 12);
    DrawText(valText,
             (int)(px + pw - valW - 8),
             (int)(py + ph - 18),
             12,
             (Color){120, 180, 255, 180});
}