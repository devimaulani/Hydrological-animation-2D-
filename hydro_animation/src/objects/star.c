#include "star.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include "../simulation/simulation_state.h"

/*
 * star.c - Implementasi Sistem Bintang Kerlap-kerlip
 *
 * FIX:
 *  1. Posisi bintang menggunakan koordinat LAYAR langsung (pixel),
 *     bukan sistem koordinat Cartesian proyek.
 *  2. Threshold diturunkan ke 0.15f agar bintang tampil sejak fase Advection.
 *  3. Lerp speed dipercepat agar fade in/out terasa smooth tapi responsif.
 */

static Star stars[STAR_COUNT];
static int  sw_cached = 1400;
static int  sh_cached = 480;

static inline float LerpF(float a, float b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return a + (b - a) * t;
}

/* PRNG deterministik - posisi bintang konsisten setiap run */
static float PseudoRand(int seed, int salt) {
    unsigned int v = (unsigned int)(seed * 2654435761u) ^ (unsigned int)(salt * 40503u + 1234567u);
    v ^= (v >> 16);
    v *= 0x45d9f3bu;
    v ^= (v >> 16);
    return (float)(v & 0x00FFFFFFu) / (float)0x01000000u;
}

void InitStars(int screenW, int screenH_sky) {
    sw_cached = screenW;
    sh_cached = screenH_sky;

    for (int i = 0; i < STAR_COUNT; i++) {
        Star *s = &stars[i];

        /* Posisi pixel layar langsung - tidak bergantung G_Origin */
        s->x = PseudoRand(i, 0) * (float)screenW;
        float yNorm = PseudoRand(i, 1);
        yNorm = yNorm * yNorm;   /* Distribusi kuadrat: lebih banyak di atas */
        s->y = yNorm * (float)screenH_sky * 0.90f;

        /* Ukuran 0.7 - 2.4 px */
        float sizeSeed = PseudoRand(i, 2);
        s->size = 0.7f + sizeSeed * sizeSeed * 1.7f;

        /* Frekuensi kerlip 0.3 - 1.8 Hz */
        s->twinkleFreq  = 0.3f + PseudoRand(i, 3) * 1.5f;

        /* Fase acak 0 - 2pi */
        s->twinklePhase = PseudoRand(i, 4) * 6.2832f;

        /* Amplitudo 0.20 - 0.65 */
        s->twinkleAmp   = 0.20f + PseudoRand(i, 5) * 0.45f;

        /* Alpha dasar 0.60 - 1.0 */
        s->baseAlpha    = 0.60f + PseudoRand(i, 6) * 0.40f;

        s->currentAlpha = 0.0f;
        s->targetAlpha  = 0.0f;
    }
}

void UpdateStars(float dt, float skyDarkness) {
    /* Hitung visibilitas global berdasarkan skyDarkness */
    float visibilityFactor = 0.0f;
    if (skyDarkness > STAR_APPEAR_THRESHOLD) {
        visibilityFactor = (skyDarkness - STAR_APPEAR_THRESHOLD) /
                           (1.0f - STAR_APPEAR_THRESHOLD);
        /* Ease-in kuadrat */
        visibilityFactor = visibilityFactor * visibilityFactor;
    }

    float time = (float)GetTime();

    for (int i = 0; i < STAR_COUNT; i++) {
        Star *s = &stars[i];

        /* Dua gelombang sin berlapis untuk efek kerlip organik */
        float wave1   = sinf(time * s->twinkleFreq * 6.2832f + s->twinklePhase);
        float wave2   = sinf(time * s->twinkleFreq * 2.3f    + s->twinklePhase * 0.6f);
        float twinkle = (wave1 * 0.65f + wave2 * 0.35f + 1.0f) * 0.5f; /* [0,1] */

        s->targetAlpha = s->baseAlpha *
                         ((1.0f - s->twinkleAmp) + s->twinkleAmp * twinkle) *
                         visibilityFactor;

        /* Exponential smoothing - speed=5 agar cepat tapi mulus */
        s->currentAlpha += (s->targetAlpha - s->currentAlpha) * 5.0f * dt;

        if (s->currentAlpha < 0.0f) s->currentAlpha = 0.0f;
        if (s->currentAlpha > 1.0f) s->currentAlpha = 1.0f;
    }
}

void DrawStars(void) {
    for (int i = 0; i < STAR_COUNT; i++) {
        Star *s = &stars[i];
        if (s->currentAlpha < 0.008f) continue;

        unsigned char alpha = (unsigned char)(s->currentAlpha * 255.0f);

        if (isWireframeMode) {
            Color wireC = (Color){0, 255, 0, alpha};
            DrawCircleLinesV((Vector2){ s->x, s->y }, s->size * 1.5f, wireC);
            // Draw a diamond dot / cross for star wireframe
            DrawLine((int)s->x - 2, (int)s->y, (int)s->x + 2, (int)s->y, wireC);
            DrawLine((int)s->x, (int)s->y - 2, (int)s->x, (int)s->y + 2, wireC);
            continue;
        }

        /* Glow lembut */
        DrawCircleV((Vector2){ s->x, s->y }, s->size * 3.0f,
                    (Color){ 255, 245, 210, (unsigned char)(s->currentAlpha * 55.0f) });

        /* Halo */
        DrawCircleV((Vector2){ s->x, s->y }, s->size * 1.7f,
                    (Color){ 255, 250, 235, (unsigned char)(s->currentAlpha * 110.0f) });

        /* Inti - variasi warna */
        Color core;
        switch (i % 7) {
            case 0: core = (Color){ 255, 255, 255, alpha }; break;
            case 1: core = (Color){ 255, 248, 210, alpha }; break;
            case 2: core = (Color){ 210, 230, 255, alpha }; break;
            case 3: core = (Color){ 255, 255, 255, alpha }; break;
            case 4: core = (Color){ 255, 245, 200, alpha }; break;
            case 5: core = (Color){ 225, 238, 255, alpha }; break;
            default:core = (Color){ 255, 255, 255, alpha }; break;
        }
        DrawCircleV((Vector2){ s->x, s->y }, s->size, core);

        /* Spark tengah untuk bintang besar */
        if (s->size > 1.3f)
            DrawCircleV((Vector2){ s->x, s->y }, s->size * 0.4f,
                        (Color){ 255, 255, 255, (unsigned char)(s->currentAlpha * 210.0f) });
    }
}