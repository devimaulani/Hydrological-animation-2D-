#include "collection.h"
#include "raylib.h"
#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "../wrapper/draw_circle.h"
#include "../simulation/simulation_state.h"
#include <math.h>
#include <stddef.h>

// =========================================================
// COLLECTION MODULE IMPLEMENTATION
// Animasi air mengalir dari hulu sungai ke muara laut
// =========================================================

static const Color RIV_RIPPLE = { 180, 220, 255, 230 };

void InitCollection() {
    // Tidak ada state khusus yang perlu diinisialisasi untuk saat ini
}

void UpdateCollection(float dt) {
    // Update logika pergerakan jika diperlukan (saat ini menggunakan GetTime di Draw)
}

void DrawCollection() {
    float time = (float)GetTime();
    RiverPoint* points = GetRiverPoints();
    int segmentCount = GetRiverSegmentCount();
    
    if (points == NULL || segmentCount <= 0) return;

    // 1. Parameter Aliran & Warna (Merespon Fase)
    float flowSpeed = 0.45f; // Diperlambat agar lebih tenang (Default: 0.55f)
    int clumpCount = 10; 
    Color baseClumpCol = { 160, 210, 255, 200 }; // Normal/Sun
    
    // Warna sungai menderas & pekat saat Hujan/Koleksi
    if (currentPhase == PHASE_PRECIPITATION || currentPhase == PHASE_INFILTR_COLLECT) {
        flowSpeed = 1.4f;    
        clumpCount = 45;       
        baseClumpCol = (Color){ 60, 110, 180, 240 }; // Indigo/Navy Pekat
    }

    // 2. Gambar "Gumpalan" Air yang Menggulung (Rolling Clumps)
    for (int c = 0; c < clumpCount; c++) { 
        float phaseOffset = (float)c / (float)clumpCount;
        // Progress 0 -> 1 (Kiri ke Kanan)
        float progress = fmodf((time * flowSpeed) + phaseOffset, 1.0f);
        
        int idx = (int)(progress * (segmentCount - 4));
        if (idx < 0) idx = 0;
        
        float baseWidth = (3.5f + (idx * 0.07f));
        unsigned char alpha = (unsigned char)(baseClumpCol.a * sinf(progress * 3.14159f));
        Color clumpColor = (Color){baseClumpCol.r, baseClumpCol.g, baseClumpCol.b, alpha};

        // Gambar 3 lingkaran kecil yang berkelompok untuk membentuk satu "gumpalan"
        float gx = points[idx].x;
        float gy = points[idx].y;
        
        // Animasi "Menggulung" searah aliran (time - space) agar gelombang bergerak ke Kanan
        float roll = sinf(time * 8.0f - phaseOffset * 12.0f) * 2.2f;
        
        // Lingkaran utama (tengah)
        Wrapper_DrawCircleFilled(gx, gy + roll, baseWidth * 1.15f, clumpColor);
        
        // Lingkaran satelit (kiri-kanan) untuk volume gumpalan
        Color accentCol = (Color){255, 255, 255, (unsigned char)(alpha * 0.5f)};
        Wrapper_DrawCircleFilled(gx - 3.0f, gy + roll - 1.5f, baseWidth * 0.7f, clumpColor);
        Wrapper_DrawCircleFilled(gx + 3.0f, gy + roll + 1.0f, baseWidth * 0.65f, clumpColor);
        
        // Kilauan di puncak gumpalan (Highlight)
        if (idx % 6 == 0) {
            Wrapper_DrawCircleFilled(gx, gy + roll + 1.5f, baseWidth * 0.4f, accentCol);
        }
    }
}
