#include "advection.h"
#include "../wrapper/draw_line.h"
#include "../simulation/simulation_state.h"
#include "coords.h"
#include <math.h>

// =========================
// POSISI & KONTROL
// =========================
// Angin sedikit lebih cepat dari awan (70.0f) agar melintas mendahului
#define WIND_SPEED 120.0f

// Posisi scroll angin - per siklus hanya satu kali melintas
static float windScrollX = 0.0f;

// Apakah angin sudah selesai melintas dalam siklus ini
static int windFinished = 0;

// =========================
void InitAdvection() {
    windScrollX = 0.0f;
    windFinished = 0;
}

void ResetAdvection() {
    windScrollX = 0.0f;
    windFinished = 0;
}

// =========================
void UpdateAdvection(float dt) {
    if (currentPhase == PHASE_ADVECTION || currentPhase == PHASE_PRECIPITATION) {
        if (!windFinished) {
            windScrollX += WIND_SPEED * dt;
            
            // Angin hanya melintas sekali, kemudian berhenti sampai siklus baru
            if (windScrollX > 1800.0f) {
                windFinished = 1;
            }
        }
    }
}

// 🔥 DRAW SPIRAL (Pusaran Angin)
// Menggambar pusaran melingkar di ujung angin menggunakan titik-titik parametrik
static void DrawWindSpiral(float x, float y, float radius, float phaseShift, Color color) {
    float time = (float)GetTime();
    int segments = 24;
    // Rotasi spiral secara aktif
    float rotation = time * 6.0f + phaseShift; 
    
    // Denyut halus pada radius spiral
    float pulseRadius = radius * (1.0f + 0.1f * sinf(time * 4.0f + phaseShift));

    float prevX = x + pulseRadius * cosf(rotation);
    float prevY = y + pulseRadius * sinf(rotation);

    for (int i = 1; i <= segments; i++) {
        float t = (float)i / segments;
        // Radius mengecil ke dalam untuk efek spiral
        float r = pulseRadius * (1.1f - t * 0.95f);
        // Sudut berputar (1.6 putaran)
        float angle = rotation + (t * 6.28f * 1.6f);
        
        float curX = x + r * cosf(angle);
        float curY = y + r * sinf(angle);

        // Alpha memudar ke ujung dalam spiral
        Color stepCol = color;
        stepCol.a = (unsigned char)(color.a * (1.0f - t * 0.6f));
        
        Wrapper_DrawLineThick(prevX, prevY, curX, curY, 2, stepCol);
        
        prevX = curX;
        prevY = curY;
    }
}

// 🔥 DRAW NATURAL WIND WISP
// Menghasilkan tiupan angin dengan ekor meliuk dan pusaran spiral di depan
static void DrawWindWisp(float cloudX, float relY, float length, float swirlRadius, float phaseShift) {
    float time = (float)GetTime();
    int tailSegments = 25;
    float tailStep = length / tailSegments;

    // Posisi angin: Memimpin di depan awan (cloudX adalah posisi awan utama)
    float headX = cloudX - 120.0f - (windScrollX * 0.6f);
    // Gerakan vertikal angin secara keseluruhan
    float headY = relY + sinf(time * 1.5f + phaseShift) * 15.0f;

    // 1. Gambar Pusaran (Spiral) Berotasi Aktif
    unsigned char alpha = (unsigned char)(160 * (1.0f - (windScrollX / 1900.0f)));
    if (windScrollX > 1900.0f) alpha = 0;
    Color windCol = (Color){220, 235, 255, alpha};

    DrawWindSpiral(headX, headY, swirlRadius, phaseShift, windCol);

    // 2. Gambar Ekor (Tail) Meliuk Bergelombang (Wavy Flow)
    float prevX = headX;
    float prevY = headY;

    for (int i = 1; i <= tailSegments; i++) {
        float t = (float)i / tailSegments;
        // Ekor memanjang ke arah berlawanan gerak
        float x = headX + (i * tailStep);
        
        // Gelombang yang merambat (propagating wave) dari kepala ke ekor
        float waveAmplitude = 18.0f * (1.0f - t * 0.4f);
        float wavePhase = x * 0.025f - time * 6.0f + phaseShift;
        float y = headY + sinf(wavePhase) * waveAmplitude;

        Color stepCol = windCol;
        stepCol.a = (unsigned char)(alpha * (0.9f - t * 0.8f));

        Wrapper_DrawLineThick(prevX, prevY, x, y, 2, stepCol);
        
        prevX = x;
        prevY = y;
    }
}

// 🔥 DRAW FINAL
void DrawAdvection() {
    if (currentPhase != PHASE_ADVECTION && currentPhase != PHASE_PRECIPITATION) return;
    if (windFinished) return;

    // Mendapatkan posisi awan laut untuk sinkronisasi dorongan
    // Angin bervariasi ukuran dan posisinya mengikuti awan dominan
    float cx = (mainClouds[0].x < 1500) ? mainClouds[0].x : RIGHT;

    // Beberapa tiupan angin bervariasi (Besar, Kecil, Panjang, Pendek)
    DrawWindWisp(cx, TOP - 80.0f,  300.0f, 15.0f, 0.0f);   // Utama
    DrawWindWisp(cx, TOP - 140.0f, 400.0f, 12.0f, 1.5f);  // Lebih panjang, spiral kecil
    DrawWindWisp(cx, TOP - 200.0f, 250.0f, 20.0f, 3.2f);  // Pendek, spiral besar
    DrawWindWisp(cx, TOP - 50.0f,  350.0f, 10.0f, 4.8f);  // Tipis di atas
    DrawWindWisp(cx, TOP - 260.0f, 320.0f, 14.0f, 6.1f);  // Di bawah
}

// 🔥 IsWindReachedLand (Status angin)
int IsWindReachedLand() {
    return currentPhase == PHASE_ADVECTION || currentPhase == PHASE_PRECIPITATION;
}