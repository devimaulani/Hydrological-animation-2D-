#include "advection.h"
#include "../wrapper/draw_line.h"
#include "../simulation/simulation_state.h"
#include "coords.h"
#include <math.h>

// =========================
// POSISI & KONTROL
// =========================
#define WIND_SPEED 220.0f  // Jauh lebih cepat dari awan (75.0f) untuk visual "dorongan" kuat

static float windScrollX = 0.0f;
static bool windFinished = false;

void InitAdvection() {
    windScrollX = 0.0f;
    windFinished = false;
}

void ResetAdvection() {
    windScrollX = 0.0f;
    windFinished = false;
}

void UpdateAdvection(float dt) {
    if (currentPhase == PHASE_ADVECTION) {
        if (!windFinished) {
            windScrollX += WIND_SPEED * dt;
            // Jika sudah melintasi seluruh layar (~1500 unit), tandai selesai
            if (windScrollX > 1500.0f) windFinished = true;
        }
    } else if (currentPhase == PHASE_INFILTR_COLLECT) {
        // Angin akan berhembus kembali saat awan keluar (opsional, tapi bagus untuk dinamika)
        windScrollX += WIND_SPEED * 0.8f * dt;
    }
}

// 🔥 DRAW NATURAL WIND (Wisp - Lebih Jelas & Cerah)
static void DrawWindWisp(float baseX, float baseY, float length, float phaseShift) {
    float time = (float)GetTime();
    int segments = 28;
    float segmentLen = length / segments;

    float currentBaseX = baseX - windScrollX;
    
    float headX = currentBaseX;
    float headY = baseY + sinf(headX * 0.015f + time * 4.5f + phaseShift) * 15.0f;

    // GAMBAR SPIRAL DI KEPALA ANGIN
    float spiralPrevX = headX;
    float spiralPrevY = headY;
    float totalAngle = PI * 2.8f;
    for (int i = 1; i <= 24; i++) {
        float t = (float)i / 24.0f;
        float r = 12.0f * (1.0f - t); 
        float angle = t * totalAngle; 
        
        // Pusat spiral ditaruh sedikit di depan (kiri) ujung badan angin
        float centerX = headX - 12.0f;
        float centerY = headY; 
        
        float sx = centerX + r * cosf(angle);
        float sy = centerY + r * sinf(angle);
        
        unsigned char alpha = (unsigned char)(220 * (1.0f - t));
        Color col = (Color){ 255, 255, 255, alpha };
        
        if (isWireframeMode && applyWireframeColor) {
            col = (Color){0, 255, 0, alpha};
        }
        
        Wrapper_DrawLineThick(spiralPrevX, spiralPrevY, sx, sy, 2.0f, col);
        spiralPrevX = sx;
        spiralPrevY = sy;
    }

    // GAMBAR BADAN ANGIN (Gelombang)
    float prevX = headX;
    float prevY = headY;

    for (int i = 0; i < segments; i++) {
        float t = (float)i / segments;
        float x = currentBaseX - (i * segmentLen);
        
        float wave = sinf(x * 0.015f + time * 4.5f + phaseShift) * 15.0f;
        float y = baseY + wave;

        // Peningkatan Visibilitas (Putih Terang dengan Alpha lebih tinggi)
        unsigned char alpha = (unsigned char)(220 * (1.0f - t));
        Color col = (Color){ 255, 255, 255, alpha }; 

        if (i > 0) {
            // Tebal 2 -> lebih jelas
            if (isWireframeMode && applyWireframeColor) col = (Color){0, 255, 0, alpha};
            Wrapper_DrawLineThick(prevX, prevY, x, y, 2.0f, col);
        }

        prevX = x;
        prevY = y;
    }
}

void DrawAdvection() {
    // Gambar angin selama fase Adveksi atau saat awan mulai keluar (koleksi)
    if (currentPhase != PHASE_ADVECTION && currentPhase != PHASE_INFILTR_COLLECT) return;
    if (windFinished && currentPhase == PHASE_ADVECTION) return;

    // Gambar 4 lapisan angin yang mendahului awan
    DrawWindWisp(RIGHT + 300.0f, TOP - 100.0f, 400.0f, 0.0f);
    DrawWindWisp(RIGHT + 600.0f, TOP - 150.0f, 500.0f, 2.0f);
    DrawWindWisp(RIGHT + 200.0f, TOP - 200.0f, 450.0f, 4.0f);
    DrawWindWisp(RIGHT + 500.0f, TOP - 250.0f, 420.0f, 6.0f);
}

int IsWindReachedLand() {
    // Status angin untuk logika mendung
    return (currentPhase == PHASE_ADVECTION && windScrollX > 400.0f);
}