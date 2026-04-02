#include "precipitation.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include "coords.h"
#include "../animation/advection.h"
#include "../animation/evaporation.h"
#include "../simulation/simulation_state.h"
#include <stdlib.h>
#include <math.h>

// =========================
// AREA HUJAN
// Koordinat: Y ke atas = positif (seperti dunia nyata)
// Hujan jatuh = y berkurang
// =========================
#define CLOUD_X      (LEFT + 330)
#define CLOUD_Y      (TOP  - 250)   // batas atas hujan (di bawah awan)
#define GROUND_Y     (BOTTOM + 300)  // batas bawah hujan (tanah)

#define RAIN_SPREAD  130
#define RAIN_COUNT   28

#define SPEED_MIN    80.0f
#define SPEED_MAX   150.0f

// =========================
typedef struct {
    float x;
    float y;
    float speed;
    float size;    // 0.7 (kecil) .. 1.3 (besar)
} Drop;

static Drop rain[RAIN_COUNT];

// =========================
static void ResetDrop(int i) {
    rain[i].x     = CLOUD_X + (rand() % (RAIN_SPREAD * 2)) - RAIN_SPREAD;
    rain[i].y     = CLOUD_Y - (rand() % 30);       // muncul sedikit di bawah awan
    rain[i].speed = SPEED_MIN + (rand() % (int)(SPEED_MAX - SPEED_MIN));
    rain[i].size  = 0.7f + (float)(rand() % 7) / 10.0f;
}

// =========================
void InitRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        ResetDrop(i);
        // Sebar posisi awal vertikal: antara awan dan tanah
        float range = CLOUD_Y - GROUND_Y;
        rain[i].y   = CLOUD_Y - (rand() % (int)range);
    }
}

// =========================
void UpdateRain(float dt) {
    // Dinamis: jumlah tetesan yang diproses sesuai tingginya angka penguapan
    int activeRainCount = (int)(RAIN_COUNT * (evaporationRate / 2.0f));
    if (activeRainCount > RAIN_COUNT) activeRainCount = RAIN_COUNT;

    for (int i = 0; i < activeRainCount; i++) {

        // Y berkurang = jatuh ke bawah (koordinat dunia nyata)
        rain[i].y -= rain[i].speed * dt;

        // Sampai tanah → reset ke atas
        if (rain[i].y < GROUND_Y) {
            ResetDrop(i);
        }
    }
}

// =========================
// TEARDROP NATURAL:
//   - Bawah : setengah lingkaran (bulat)
//   - Atas  : dua sisi melengkung bertemu lancip di puncak
//
// Dalam koordinat layar (y bawah = lebih besar secara piksel),
// tetes jatuh → kepala bulat di BAWAH, lancip di ATAS.
// =========================
static void DrawDrop(int cx, int cy, float s) {

    Color fill    = (Color){ 80, 185, 220, 220};
    Color outline = (Color){ 45, 130, 175, 255};
    Color shine   = (Color){200, 235, 255, 160};  // highlight putih

    int r = (int)(6.0f * s);   // radius bagian bulat atas

    // Dalam piksel layar: cy = pusat lingkaran atas
    // Lancip ada di BAWAH piksel = cy + r + tipH
    int tipH = (int)(14.0f * s);
    int tipY = cy + r + tipH;   // piksel: lebih besar = lebih bawah layar

    // ── ISI badan tetes (area lancip di bawah lingkaran) ──
    // Gambar kolom dari batas bawah lingkaran (cy + r) sampai bawah (tipY)
    for (int dx = -r; dx <= r; dx++) {
        // Lebar pada setiap ketinggian: melebar dari 0 (di tipY) ke r (di cy+r)
        float ratio = fabsf((float)dx / (float)r);
        // y mulai pada batas bawah lingkaran, berakhir pada ketinggian sesuai lebar dx
        int y_start = cy + r;
        int y_end   = tipY - (int)(tipH * ratio);
        if (y_end > y_start)
            Wrapper_DrawLineThick(cx + dx, y_start, cx + dx, y_end, 1, fill);
    }

    // ── Lingkaran atas (bulat) ──
    Wrapper_DrawCircleFilled(cx, cy, r, fill);

    // ── Outline sisi kiri (dari tepi kiri lingkaran ke puncak lancip bawah) ──
    float px = (float)(cx - r), py = (float)cy;
    for (int k = 1; k <= 10; k++) {
        float t  = (float)k / 10.0f;
        // Bezier kuadratik: tepi kiri (cx-r, cy) → kontrol (cx-r, cy+r) → lancip (cx, tipY)
        float bx = (1-t)*(1-t)*(cx - r) + 2*(1-t)*t*(cx - r) + t*t*cx;
        float by = (1-t)*(1-t)*cy + 2*(1-t)*t*(cy + r) + t*t*tipY;
        Wrapper_DrawLineThick((int)px,(int)py,(int)bx,(int)by, 1, outline);
        px = bx; py = by;
    }

    // ── Outline sisi kanan (mirror) ──
    px = (float)(cx + r); py = (float)cy;
    for (int k = 1; k <= 10; k++) {
        float t  = (float)k / 10.0f;
        float bx = (1-t)*(1-t)*(cx + r) + 2*(1-t)*t*(cx + r) + t*t*cx;
        float by = (1-t)*(1-t)*cy + 2*(1-t)*t*(cy + r) + t*t*tipY;
        Wrapper_DrawLineThick((int)px,(int)py,(int)bx,(int)by, 1, outline);
        px = bx; py = by;
    }

    // ── Outline atas (setengah lingkaran atas) ──
    // Digambar oleh DrawCircleFilled — tambahkan arc outline saja
    for (int deg = 180; deg <= 360; deg += 10) {
        float a1 = deg       * 3.14159f / 180.0f;
        float a2 = (deg + 10)* 3.14159f / 180.0f;
        int ox1 = cx + (int)(r * cosf(a1));
        int oy1 = cy + (int)(r * sinf(a1));
        int ox2 = cx + (int)(r * cosf(a2));
        int oy2 = cy + (int)(r * sinf(a2));
        Wrapper_DrawLineThick(ox1, oy1, ox2, oy2, 1, outline);
    }

    // ── Highlight kecil (kilauan air) ──
    Wrapper_DrawCircleFilled(cx - r/3, cy - r/3, (int)(r * 0.3f), shine);
}

// =========================
void DrawRain() {
    // Hitung ulang untuk sinkronisasi render
    int activeRainCount = (int)(RAIN_COUNT * (evaporationRate / 2.0f));
    if (activeRainCount > RAIN_COUNT) activeRainCount = RAIN_COUNT;

    for (int i = 0; i < activeRainCount; i++) {
        DrawDrop((int)rain[i].x, (int)rain[i].y, rain[i].size);
    }
}
