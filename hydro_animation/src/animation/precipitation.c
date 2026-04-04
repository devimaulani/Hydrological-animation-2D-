#include "precipitation.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include "coords.h"
#include "../animation/advection.h"
#include "../animation/evaporation.h"
#include "../simulation/simulation_state.h"
#include <stdlib.h>
#include <math.h>


#define CLOUD_Y      (TOP  - 250)   // batas atas hujan (di bawah awan)
#define GROUND_Y     (BOTTOM + 240)  // diperpanjang sedikit ke bawah agar ke daratan


#define RAIN_COUNT   48

#define SPEED_MIN    80.0f
#define SPEED_MAX   150.0f


#define SPLASH_COUNT  16
#define SPLASH_LIFETIME 0.5f  // Waktu hidup percikan dalam detik

typedef struct {
    float x;
    float y;
    float timer;      // Waktu sejak percikan muncul
    float size;        // Ukuran lengkungan percikan
    int active;        // Apakah percikan sedang aktif
} Splash;

static Splash splashes[SPLASH_COUNT];

typedef struct {
    float x;
    float y;
    float speed;
    float size;    // 0.7 (kecil) .. 1.3 (besar)
    float targetY; // Batas jatuh dinamis (Dua Wilayah)
} Drop;

static Drop rain[RAIN_COUNT];

static float GetRainLeft() {
    // Cloud[1] berada lebih ke kiri (cloudStopX - 200)
    return cloudStopX - 200.0f - 140.0f;  // Extra padding ke kiri
}

static float GetRainRight() {
    // Cloud[0] berada paling kanan (cloudStopX)
    return cloudStopX + 140.0f;  // Extra padding ke kanan
}

static void ResetDrop(int i) {
    // Posisi X menyebar merata di seluruh area kedua awan
    float rainLeft = GetRainLeft();
    float rainRight = GetRainRight();
    float rainWidth = rainRight - rainLeft;
    
    float nx = rainLeft + (float)(rand() % (int)rainWidth);
    rain[i].x     = nx;
    rain[i].y     = CLOUD_Y - (rand() % 30);       // muncul sedikit di bawah awan
    rain[i].speed = SPEED_MIN + (rand() % (int)(SPEED_MAX - SPEED_MIN));
    rain[i].size  = 0.7f + (float)(rand() % 7) / 10.0f;

    // --- PEMBAGIAN DUA WILAYAH (Y target) ---
    // Posisi ditukar sesuai permintaan: Kiri jadi panjang, Kanan jadi pendek
    if (nx < cloudStopX - 100.0f) {
        rain[i].targetY = BOTTOM + 230.0f; // Jalur Panjang (Target Rendah)
    } else {
        rain[i].targetY = BOTTOM + 330.0f; // Jalur Pendek (Target Tinggi)
    }
}

// Aktifkan percikan air di posisi tertentu
static void SpawnSplash(float x, float y) {
    for (int i = 0; i < SPLASH_COUNT; i++) {
        if (!splashes[i].active) {
            splashes[i].x = x;
            splashes[i].y = y;
            splashes[i].timer = 0.0f;
            splashes[i].size = 4.0f + (float)(rand() % 6);  // Ukuran lebih besar
            splashes[i].active = 1;
            return;
        }
    }
}

void InitRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        ResetDrop(i);
        // Sebar posisi awal vertikal secara acak
        float range = CLOUD_Y - (BOTTOM + 330.0f);
        rain[i].y   = CLOUD_Y - (rand() % (int)range);
    }
    // Inisialisasi semua percikan sebagai tidak aktif
    for (int i = 0; i < SPLASH_COUNT; i++) {
        splashes[i].active = 0;
    }
}

void UpdateRain(float dt) {
    // Dinamis: jumlah tetesan yang diproses sesuai tingginya angka penguapan
    int activeRainCount = (int)(RAIN_COUNT * (evaporationRate / 2.0f));
    if (activeRainCount > RAIN_COUNT) activeRainCount = RAIN_COUNT;

    for (int i = 0; i < activeRainCount; i++) {

        // Y berkurang = jatuh ke bawah (koordinat dunia nyata)
        rain[i].y -= rain[i].speed * dt;

        // Sampai "tanah" spesifik wilayahnya → spawn percikan + reset ke atas
        if (rain[i].y < rain[i].targetY) {
            // Buat percikan air di lokasi tetesan jatuh (sesuai targetY partikel tersebut)
            SpawnSplash(rain[i].x, rain[i].targetY);
            ResetDrop(i);
        }
    }

    // Update semua percikan yang aktif
    for (int i = 0; i < SPLASH_COUNT; i++) {
        if (splashes[i].active) {
            splashes[i].timer += dt;
            if (splashes[i].timer >= SPLASH_LIFETIME) {
                splashes[i].active = 0;
            }
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


static void DrawSplash(Splash* sp) {
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
    // Hitung ulang untuk sinkronisasi render
    int activeRainCount = (int)(RAIN_COUNT * (evaporationRate / 2.0f));
    if (activeRainCount > RAIN_COUNT) activeRainCount = RAIN_COUNT;

    for (int i = 0; i < activeRainCount; i++) {
        DrawDrop(rain[i].x, rain[i].y, rain[i].size);
    }
    
    // Gambar semua percikan yang aktif
    for (int i = 0; i < SPLASH_COUNT; i++) {
        DrawSplash(&splashes[i]);
    }
}
