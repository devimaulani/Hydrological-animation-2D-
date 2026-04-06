#include "precipitation.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include "coords.h"
#include "../animation/advection.h"
#include "../animation/evaporation.h"
#include "../simulation/simulation_state.h"
#include <stdlib.h>
#include <math.h>

#define RAIN_COUNT   60     // Ditambah agar lebih merata
#define SPLASH_COUNT 35
#define SPEED_MIN    220.0f
#define SPEED_MAX    380.0f

#define CLOUD_Y      (TOP - 220)
#define SPLASH_LIFETIME 0.4f

typedef struct {
    float x;
    float y;
    float speed;
    float size;
    float targetY; // Per awan (Pendek vs Panjang)
} Drop;

typedef struct {
    float x, y, timer, size;
    bool active;
} Splash;

static Drop rain[RAIN_COUNT];
static Splash splashes[SPLASH_COUNT];

static void ResetDrop(int i) {
    // Memilih antara Cloud 0 (Kanan) atau Cloud 1 (Kiri) agar penyebaran merata
    int cloudIdx = i % 2; 
    float spread = 120.0f;
    
    rain[i].x = mainClouds[cloudIdx].x + (rand() % (int)(spread * 2)) - spread;
    
    // Pastikan hujan tidak melebihi batas darat (X = 80.0f) agar tidak jatuh di tebing/laut
    if (rain[i].x > 80.0f) {
        rain[i].x = 80.0f - (rand() % 40);
    }
    
    rain[i].y = mainClouds[cloudIdx].y - 20.0f - (rand() % 30);
    rain[i].speed = SPEED_MIN + (rand() % (int)(SPEED_MAX - SPEED_MIN));
    rain[i].size = 0.7f + (float)(rand() % 6) / 10.0f;

    // --- PEMBAGIAN DUA WILAYAH (Y target) ---
    // Awan Pertama (Cloud 0, Kanan): Y lebih pendek (Target Tinggi)
    // Awan Kedua (Cloud 1, Kiri): Y lebih panjang (Target Rendah)
    if (cloudIdx == 1) {
        rain[i].targetY = BOTTOM + 240.0f; // Jalur Panjang
    } else {
        rain[i].targetY = BOTTOM + 330.0f; // Jalur Pendek
    }
}

static void SpawnSplash(float x, float y) {
    for (int i = 0; i < SPLASH_COUNT; i++) {
        if (!splashes[i].active) {
            splashes[i].active = true;
            splashes[i].x = x;
            splashes[i].y = y;
            splashes[i].timer = 0.0f;
            splashes[i].size = 4.0f + (rand() % 5);
            return;
        }
    }
}

void InitRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        ResetDrop(i);
        // Sebar awal
        rain[i].y = (float)(rand() % (int)(TOP - rain[i].targetY)) + rain[i].targetY;
    }
    for (int i = 0; i < SPLASH_COUNT; i++) splashes[i].active = false;
}

void UpdateRain(float dt) {
    if (currentPhase != PHASE_PRECIPITATION && currentPhase != PHASE_INFILTR_COLLECT) return;

    for (int i = 0; i < RAIN_COUNT; i++) {
        rain[i].y -= rain[i].speed * dt;

        if (rain[i].y < rain[i].targetY) {
            if (rain[i].x < 80.0f) { // Efek daratan
                SpawnSplash(rain[i].x, rain[i].targetY);
            }
            ResetDrop(i);
        }
    }

    for (int i = 0; i < SPLASH_COUNT; i++) {
        if (splashes[i].active) {
            splashes[i].timer += dt;
            if (splashes[i].timer >= SPLASH_LIFETIME) splashes[i].active = false;
        }
    }
}

static void DrawDrop(float cx, float cy, float s) {
    Color fill    = (Color){ 80, 185, 220, 220};
    Color outline = (Color){ 45, 130, 175, 255};
    Color shine   = (Color){200, 235, 255, 160};

    float r = 6.0f * s;
    float tipH = 14.0f * s;
    float tipY = cy + r + tipH;

    for (float dx = -r; dx <= r; dx += 1.0f) {
        float ratio = fabsf(dx / r);
        float y_start = cy + r;
        float y_end   = tipY - (tipH * ratio);
        if (y_end > y_start)
            Wrapper_DrawLineThick(cx + dx, y_start, cx + dx, y_end, 1.0f, fill);
    }

    Wrapper_DrawCircleFilled(cx, cy, r, fill);

    float px = cx - r, py = cy;
    for (int k = 1; k <= 10; k++) {
        float t  = (float)k / 10.0f;
        float bx = (1-t)*(1-t)*(cx - r) + 2*(1-t)*t*(cx - r) + t*t*cx;
        float by = (1-t)*(1-t)*cy + 2*(1-t)*t*(cy + r) + t*t*tipY;
        Wrapper_DrawLineThick(px, py, bx, by, 1.0f, outline);
        px = bx; py = by;
    }

    px = cx + r; py = cy;
    for (int k = 1; k <= 10; k++) {
        float t  = (float)k / 10.0f;
        float bx = (1-t)*(1-t)*(cx + r) + 2*(1-t)*t*(cx + r) + t*t*cx;
        float by = (1-t)*(1-t)*cy + 2*(1-t)*t*(cy + r) + t*t*tipY;
        Wrapper_DrawLineThick(px, py, bx, by, 1.0f, outline);
        px = bx; py = by;
    }

    for (int deg = 180; deg <= 360; deg += 10) {
        float a1 = deg       * 3.14159f / 180.0f;
        float a2 = (deg + 10)* 3.14159f / 180.0f;
        float ox1 = cx + (r * cosf(a1));
        float oy1 = cy + (r * sinf(a1));
        float ox2 = cx + (r * cosf(a2));
        float oy2 = cy + (r * sinf(a2));
        Wrapper_DrawLineThick(ox1, oy1, ox2, oy2, 1.0f, outline);
    }

    Wrapper_DrawCircleFilled(cx - r/3.0f, cy - r/3.0f, r * 0.3f, shine);
}

static void DrawSplashEffect(Splash* sp) {
    if (!sp->active) return;
    
    // Progress animasi percikan (0.0 = baru muncul, 1.0 = selesai)
    float progress = sp->timer / SPLASH_LIFETIME;
    
    // Alpha memudar seiring waktu
    unsigned char alpha = (unsigned char)(240 * (1.0f - progress));
    if (alpha < 5) return;
    
    // Warna lebih cerah dan kontras agar jelas terlihat
    Color splashColor = (Color){180, 225, 255, alpha};
    
    // Ukuran percikan membesar lalu mengecil
    float sizeMultiplier = sinf(progress * 3.14159f);  // naik lalu turun
    float splashRadius = sp->size * sizeMultiplier;
    
    if (splashRadius < 0.5f) return;
    
    float cx = sp->x;
    float cy = sp->y;
    
    // === Percikan utama - lengkungan besar ke atas ===
    float prevArcX = cx - splashRadius;
    float prevArcY = cy;
    int arcSegments = 12;
    for (int k = 1; k <= arcSegments; k++) {
        float t = (float)k / arcSegments;
        float angle = 3.14159f * t;  // 0 sampai pi (setengah lingkaran atas)
        float arcX = cx - splashRadius * cosf(angle);
        float arcY = cy + splashRadius * 0.7f * sinf(angle);  // ke atas (Y positif)
        
        Wrapper_DrawLineThick(prevArcX, prevArcY, arcX, arcY, 2.0f, splashColor);
        prevArcX = arcX;
        prevArcY = arcY;
    }
    
    // === Percikan kecil di kiri ===
    float smallR = splashRadius * 0.55f;
    float offsetX = splashRadius * 1.4f;
    
    unsigned char smallAlpha = (unsigned char)(alpha * 0.7f);
    Color smallColor = (Color){180, 225, 255, smallAlpha};
    
    prevArcX = cx - offsetX - smallR;
    prevArcY = cy;
    for (int k = 1; k <= arcSegments; k++) {
        float t = (float)k / arcSegments;
        float angle = 3.14159f * t;
        float arcX = (cx - offsetX) - smallR * cosf(angle);
        float arcY = cy + smallR * 0.6f * sinf(angle);
        
        Wrapper_DrawLineThick(prevArcX, prevArcY, arcX, arcY, 1.5f, smallColor);
        prevArcX = arcX;
        prevArcY = arcY;
    }
    
    // === Percikan kecil di kanan ===
    prevArcX = cx + offsetX - smallR;
    prevArcY = cy;
    for (int k = 1; k <= arcSegments; k++) {
        float t = (float)k / arcSegments;
        float angle = 3.14159f * t;
        float arcX = (cx + offsetX) - smallR * cosf(angle);
        float arcY = cy + smallR * 0.6f * sinf(angle);
        
        Wrapper_DrawLineThick(prevArcX, prevArcY, arcX, arcY, 1.5f, smallColor);
        prevArcX = arcX;
        prevArcY = arcY;
    }
    
    // === Titik-titik kecil percikan tambahan agar lebih hidup ===
    if (progress < 0.5f) {
        float dotAlpha = (float)alpha * (1.0f - progress * 2.0f);
        Color dotColor = (Color){200, 235, 255, (unsigned char)dotAlpha};
        float dotR = 1.5f * sizeMultiplier;
        // Titik percikan di atas kiri dan kanan
        Wrapper_DrawCircleFilled(cx - splashRadius * 0.8f, cy + splashRadius * 0.9f, dotR, dotColor);
        Wrapper_DrawCircleFilled(cx + splashRadius * 0.8f, cy + splashRadius * 0.9f, dotR, dotColor);
        Wrapper_DrawCircleFilled(cx, cy + splashRadius * 1.1f, dotR * 0.8f, dotColor);
    }
}

void DrawRain() {
    if (currentPhase != PHASE_PRECIPITATION && currentPhase != PHASE_INFILTR_COLLECT) return;

    for (int i = 0; i < RAIN_COUNT; i++) {
        DrawDrop(rain[i].x, rain[i].y, rain[i].size);
    }
    for (int i = 0; i < SPLASH_COUNT; i++) {
        if (splashes[i].active) DrawSplashEffect(&splashes[i]);
    }
}
