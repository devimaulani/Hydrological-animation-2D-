#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "coords.h"
#include <math.h>

// =========================================================
// RIVER MODULE (Pengganti Lake) - Sungai Bentuk Natural
// Menggunakan lingkaran-lingkaran kecil yang menumpuk untuk
// menggambar garis halus tanpa patahan sudut kotak.
// =========================================================

#define RIVER_SEGMENTS 120

typedef struct { float x; float y; } RiverPoint;
static RiverPoint riverPoints[RIVER_SEGMENTS];

static const Color RIV_BLUE_DARK  = { 40,  120, 200, 220 };
static const Color RIV_BLUE_LIGHT = { 80,  160, 240, 240 };
static const Color RIV_RIPPLE     = { 180, 220, 255, 230 };

void InitLake() {
    float startX = LEFT + 50; 
    float startY = -112; 
    
    // Hitung posisi bibir pantai (shoreline) secara dinamis agar sungai menyatu sempurna
    int landTop = (int)(-HALF_H / 4.2f);
    float endY = BOTTOM + 230; 
    float tShore = (float)(landTop - endY) / (float)(landTop - BOTTOM);
    float endX = 100.0f + (100.0f * tShore) + (25.0f * sinf(endY * 0.08f)) + (10.0f * cosf(endY * 0.2f));
    
    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        float t = (float)i / (RIVER_SEGMENTS - 1); 
        
        float px = startX + t * (endX - startX);
        float py = startY + t * (endY - startY);
        
        // Membentuk huruf S yang lebih dinamis dan tak turun terlalu curam
        float meanderX = 0;
        float meanderY = sinf(t * 3.14159f * 2.5f) * 15.0f; // Kurva lebih landai
        
        riverPoints[i].x = px + meanderX;
        riverPoints[i].y = py + meanderY;
    }
}

extern double GetTime(void); // dari raylib (atau disuplai otomatis)

void DrawLake() {
    float time = (float)GetTime();

    // 1. Gambar Dasar Sungai (Overlapping Solid Circles - Sangat Halus)
    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        // Lebar sungai (melebar di akhir/muara)
        float width = 6.0f + (i * 0.25f); 
        Wrapper_DrawEllipseFilled((int)riverPoints[i].x, (int)riverPoints[i].y, (int)width, (int)(width * 0.9f), RIV_BLUE_DARK);
    }
    
    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        float width = 4.0f + (i * 0.22f); 
        Wrapper_DrawEllipseFilled((int)riverPoints[i].x, (int)riverPoints[i].y, (int)width, (int)(width * 0.9f), RIV_BLUE_LIGHT);
    }
    
    // 2. Animasi Air Bergelembung Mengalir (Ripples)
    for (int r = 0; r < 6; r++) { 
        float rippleProgress = fmodf((time * 0.25f) + (r * 0.166f), 1.0f);
        int idx = (int)(rippleProgress * (RIVER_SEGMENTS - 1));
        
        float width = 3.0f + (idx * 0.06f);
        Wrapper_DrawEllipseFilled((int)riverPoints[idx].x, (int)riverPoints[idx].y, (int)width, (int)(width * 0.8f), RIV_RIPPLE);
    }
}