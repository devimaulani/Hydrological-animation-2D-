#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "../simulation/simulation_state.h"
#include "coords.h"
#include <math.h>

static RiverPoint riverPoints[RIVER_SEGMENTS];

// Warna-warna air sungai
static const Color RIV_BLUE_DARK  = { 40,  120, 200, 220 };
static const Color RIV_BLUE_LIGHT = { 80,  160, 240, 240 };

void InitLake() {
    // Jalur sungai dimulai dari dataran tinggi menuju laut
    float startX = (float)LEFT + 50.0f; 
    float startY = -112.0f; 
    
    int landTop = (int)(-HALF_H / 4.2f);
    float endY = (float)BOTTOM + 230.0f; 
    float tShore = (float)(landTop - endY) / (float)(landTop - BOTTOM);
    float endX = 100.0f + (100.0f * tShore) + (25.0f * sinf(endY * 0.08f)) + (10.0f * cosf(endY * 0.2f));
    
    // Inisialisasi titik-titik sungai dengan "meandering" (kelokan) natural
    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        float t = (float)i / (RIVER_SEGMENTS - 1); 
        float px = startX + t * (endX - startX);
        float py = startY + t * (endY - startY);
        float meanderY = sinf(t * 3.14159f * 2.5f) * 15.0f;
        
        riverPoints[i].x = px;
        riverPoints[i].y = py + meanderY;
    }
}

void DrawLake(float overflow) {
    // 1. Gambar Dasar Sungai (Lapis Bawah - Lebih Gelap)
    // Jika meluap, warna dibuat sedikit lebih pekat/keruh
    Color cDark = RIV_BLUE_DARK;
    Color cLight = RIV_BLUE_LIGHT;
    
    if (overflow > 1.05f) {
        cDark.r = (unsigned char)(cDark.r * 0.8f);
        cDark.g = (unsigned char)(cDark.g * 0.8f);
        cDark.b = (unsigned char)(cDark.b * 0.9f);
        
        cLight.r = (unsigned char)(cLight.r * 0.85f);
        cLight.g = (unsigned char)(cLight.g * 0.85f);
        cLight.b = (unsigned char)(cLight.b * 0.95f);
    }

    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        // Lebar dasar dikalikan dengan faktor luapan
        float width = (6.0f + (i * 0.25f)) * overflow; 
        Wrapper_DrawEllipseFilled(riverPoints[i].x, riverPoints[i].y, width, width * 0.9f, cDark);
    }
    
    // 2. Gambar Dasar Sungai (Lapis Atas - Lebih Terang)
    for (int i = 0; i < RIVER_SEGMENTS; i++) {
        float width = (4.0f + (i * 0.22f)) * overflow; 
        Wrapper_DrawEllipseFilled(riverPoints[i].x, riverPoints[i].y, width, width * 0.9f, cLight);
    }
}

RiverPoint* GetRiverPoints() {
    return riverPoints;
}

int GetRiverSegmentCount() {
    return RIVER_SEGMENTS;
}