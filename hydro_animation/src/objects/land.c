#include "land.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>
#include "raylib.h"


static int GetShoreX(int y) {
    int landTop = (int)(-HALF_H / 4.2f);
    float t = (float)(landTop - y) / (float)(landTop - BOTTOM);
    // Miring dari atas (100) ke bawah (200)
    int edgeX = 100 + (int)(100.0f * t);
    // Lekukan teluk natural
    edgeX += (int)(25.0f * sinf((float)y * 0.08f)) + (int)(10.0f * cosf((float)y * 0.2f));
    return edgeX;
}


static int GetSurfaceHeight(int x) {
    // 1. Lengkungan di "Kaki Gunung" (Sebelah kiri)
    float mountainFoot = 0;
    if (x < LEFT + 400) {
        float t = (float)(x - LEFT) / 400.0f;
        mountainFoot = 35.0f * powf(1.0f - t, 1.3f); // Melengkung landai dari kiri
    }

    // 2. Tekstur Tanah (Slight Bumps)
    // Dibuat lebih landai dan sedikit ke atas agar tidak ada celah berlubang (dent)
    float hills = 5.0f * sinf((float)x * 0.02f) + 3.0f * cosf((float)x * 0.04f);
    
    // Memberi sedikit tonjolan ke atas (offset positif) agar menutupi celah
    float landBase = 12.0f; 

    return (int)(mountainFoot + hills + landBase);
}


int GetLayerWave(int x) {
    // Kombinasi gelombang untuk variasi ±15px yang natural
    return (int)(12.0f * sinf((float)x * 0.012f) + 6.0f * cosf((float)x * 0.025f));
}

void DrawLand() {
    int landTop = (int)(-HALF_H / 4.2f); // Patokan dasar

    // Render dari atas ke bawah untuk mengisi seluruh volume tanah
    for (int y = landTop + 50; y > BOTTOM; y--) {
        int shoreLimit = GetShoreX(y);
        
        // Scanline horizontal per baris Y
        for (int x = LEFT; x < shoreLimit; x++) {
            int wave = GetLayerWave(x);
            int surfThresh = BOTTOM + 165 + wave;
            int soilThresh = BOTTOM + 145+ wave;

            // Memundurkan lapisan tanah bawah agar tidak menabrak laut
            int subSoilOffset = 0;
            if (y <= surfThresh) {
                subSoilOffset = 20; 
                if (x >= (shoreLimit - subSoilOffset)) continue;
            }

            int currentSurface = landTop + GetSurfaceHeight(x);
            int distToShore = shoreLimit - x;

            // Logika Landai (Sloping Shoreline)
            if (distToShore < 100) {
                float slopeFactor = (float)distToShore / 100.0f; 
                int seaLevel = landTop - 12; 
                currentSurface = seaLevel + (int)((currentSurface - seaLevel) * slopeFactor);
            }
            
            if (y > currentSurface) continue;

            Color c;
            // Penentuan Warna Lapisan (Dinamis, Organik & Bergradasi)
            if (y > surfThresh) {
                // LAPISAN TANAH HIJAU & PASIR
                c = (Color){90, 160, 85, 255}; 
                
                if (y < landTop - 5 && distToShore < 100) {
                    if (distToShore >= 40) {
                        float blend = (float)(100 - distToShore) / 60.0f;
                        c.r = (unsigned char)(90  + blend * (235 - 90));
                        c.g = (unsigned char)(160 + blend * (215 - 160));
                        c.b = (unsigned char)(85  + blend * (160 - 85));
                    } else {
                        float blend = (float)(40 - distToShore) / 40.0f;
                        c.r = (unsigned char)(235 + blend * (60  - 235));
                        c.g = (unsigned char)(215 + blend * (220 - 215));
                        c.b = (unsigned char)(160 + blend * (210 - 160));
                    }
                }
            } 
            else if (y > soilThresh) {
                // LAPISAN TRANSISI TANAH (Gradasi Hijau -> Cokelat Muted)
                float blend = (float)(y - soilThresh) / (float)(surfThresh - soilThresh);
                // Hijau: {90, 160, 85}, Cokelat: {145, 125, 105}
                c.r = (unsigned char)(145 + blend * (90  - 145));
                c.g = (unsigned char)(125 + blend * (160 - 125));
                c.b = (unsigned char)(105 + blend * (85  - 105));
                c.a = 255;
                
                if (distToShore < 15) {
                    float darkBlend = (float)(15 - distToShore) / 15.0f;
                    c.r = (unsigned char)(c.r * (1.0f - darkBlend * 0.2f));
                    c.g = (unsigned char)(c.g * (1.0f - darkBlend * 0.2f));
                    c.b = (unsigned char)(c.b * (1.0f - darkBlend * 0.2f));
                }
            } 
            else {
                // LAPISAN AKUIFER (Blue Groundwater)
                c = (Color){50, 140, 210, 255};
            }

            Wrapper_DrawLineThick(x, y, x + 1, y, 2, c);
        }
    }
}