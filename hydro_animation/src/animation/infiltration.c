#include "infiltration.h"
#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <stdlib.h>
#include <math.h>

// =========================================================
// INFILTRATION MODULE - Penyerapan Air ke Bawah Tanah
//
// Partikel air muncul dari bawah danau organik,
// lalu meresap ke bawah tanah secara vertikal.
//
// ALUR:
//   [LAKE_BOTTOM_Y]  ← spawn di dasar danau
//        |
//        v  (turun, makin pudar)
//        |
//   [INFIL_END_Y]    ← menghilang di dalam tanah
//        |
//   → reset ke dasar danau
// =========================================================

// Kedalaman maksimum partikel meresap
#define INFIL_END_Y      (BOTTOM + 50)

// -------------------------
// KONFIGURASI PARTIKEL
// -------------------------
#define PARTICLE_COUNT   30
#define PARTICLE_SPEED   40.0f
#define SPEED_VARIANCE   15

typedef struct {
    float x;
    float y;
    float speed;
    float alpha;
} Particle;

static Particle particles[PARTICLE_COUNT];


// =========================================================
// FUNGSI INTERNAL
// =========================================================

// Spawn X acak di bawah area danau organik
// Menggunakan radius horizontal danau sebagai batas
static float RandomXInsideLake() {
    // Sedikit lebih sempit dari radius agar tidak keluar blob
    int range = (int)(LAKE_RADIUS_X * 1.2f);
    return (float)(LAKE_CENTER_X - range + rand() % (range * 2));
}

// Alpha berkurang seiring kedalaman (efek meresap & menghilang)
static float CalcAlpha(float y) {
    float totalRange = (float)(LAKE_BOTTOM_Y - INFIL_END_Y);
    if (totalRange <= 0.0f) return 1.0f;
    float progress = (LAKE_BOTTOM_Y - y) / totalRange;
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    return 1.0f - progress * 0.88f;
}

// Reset partikel ke dasar danau
static void ResetParticle(int i) {
    particles[i].x     = RandomXInsideLake();
    particles[i].y     = (float)LAKE_BOTTOM_Y;
    particles[i].speed = PARTICLE_SPEED
                         + (rand() % SPEED_VARIANCE)
                         - (SPEED_VARIANCE / 2);
    particles[i].alpha = 1.0f;
}


// =========================================================
// INIT
// Sebar partikel acak di sepanjang jalur agar
// animasi tidak mulai serentak dari atas semua.
// =========================================================
void InitInfiltration() {

    float totalRange = (float)(LAKE_BOTTOM_Y - INFIL_END_Y);

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        ResetParticle(i);

        // Posisi awal acak di sepanjang jalur infiltrasi
        float offset   = (float)(rand() % (int)totalRange);
        particles[i].y = (float)LAKE_BOTTOM_Y - offset;
        particles[i].alpha = CalcAlpha(particles[i].y);
    }
}


// =========================================================
// UPDATE
// =========================================================
void UpdateInfiltration(float dt) {

    for (int i = 0; i < PARTICLE_COUNT; i++) {

        // Gerak ke bawah
        particles[i].y -= particles[i].speed * dt;

        // Pudar sesuai kedalaman
        particles[i].alpha = CalcAlpha(particles[i].y);

        // Reset jika sudah mencapai kedalaman maksimum
        if (particles[i].y <= (float)INFIL_END_Y) {
            ResetParticle(i);
        }
    }
}


// =========================================================
// DRAW
// Partikel digambar sebagai garis vertikal pendek
// yang makin pudar semakin dalam.
// =========================================================
void DrawInfiltration() {

    for (int i = 0; i < PARTICLE_COUNT; i++) {

        unsigned char a = (unsigned char)(particles[i].alpha * 210);
        Color color     = (Color){ 80, 150, 230, a };

        int x = (int)particles[i].x;
        int y = (int)particles[i].y;

        Wrapper_DrawLineThick(x, y, x, y - 7, 2, color);
    }
}