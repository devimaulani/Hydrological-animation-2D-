#include "land.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>
#include "raylib.h"

// ===================================
// UTILITY: Profil Garis Pantai (Shoreline)
// ===================================
static int GetShoreX(int y) {
    int landTop = (int)(-HALF_H / 4.2f);
    float t = (float)(landTop - y) / (float)(landTop - BOTTOM);
    // Miring dari atas (100) ke bawah (200)
    int edgeX = 100 + (int)(100.0f * t);
    // Lekukan teluk natural
    edgeX += (int)(25.0f * sinf((float)y * 0.08f)) + (int)(10.0f * cosf((float)y * 0.2f));
    return edgeX;
}

// ===================================
// UTILITY: Profil Ketinggian Permukaan (Hills & Foot of Mountain)
// ===================================
static int GetSurfaceHeight(int x) {
    // 1. Lengkungan di "Kaki Gunung" (Sebelah kiri)
    float mountainFoot = 0;
    if (x < LEFT + 300) {
        float t = (float)(x - LEFT) / 300.0f;
        mountainFoot = 25.0f * powf(1.0f - t, 1.5f); // Melengkung landai dari kiri
    }

    // 2. Undulasi Bukit (Land) agar tidak rata
    float hills = 12.0f * sinf((float)x * 0.015f) + 6.0f * cosf((float)x * 0.035f);

    return (int)(mountainFoot + hills);
}

void DrawLand() {
    int landTop = (int)(-HALF_H / 4.2f); // Patokan dasar

    // Render dari atas ke bawah untuk mengisi seluruh volume tanah
    // Mulai dari landTop + 50 untuk menampung bukit yang naik ke atas
    for (int y = landTop + 50; y > BOTTOM; y--) {
        int shoreLimit = GetShoreX(y);
        
        // Memundurkan lapisan tanah bawah agar tidak menabrak laut
        int subSoilOffset = 0;
        if (y <= BOTTOM + 180) {
            subSoilOffset = 20; // Geser mundur 60px untuk lapisan tanah & aquifer
        }
        
        // Scanline horizontal per baris Y
        // Menggunakan step kecil agar solid tanpa tekstur kasar
        for (int x = LEFT; x < (shoreLimit - subSoilOffset); x += 3) {
            int currentSurface = landTop + GetSurfaceHeight(x);
            
            // JANGAN menggambar jika di atas permukaan rumput
            if (y > currentSurface) continue;

            Color c;
            // Penentuan Warna Lapisan (Ditingkatkan)
            if (y > BOTTOM + 180) {
                // LAPISAN RUMPUT (Hiuan Natural)
                c = (Color){80, 175, 85, 255};
                
                // Efek Tepian Pantai: Gradasi ke air muda (Turquoise)
                // HANYA jika y mendekati permukaan laut ( landTop - 15 ke bawah)
                // Ini mencegah "genangan air" muncul di atas kaki gunung yang tinggi
                int distToShore = shoreLimit - x;
                if (y < landTop - 15 && distToShore < 35) {
                    float blend = (float)(35 - distToShore) / 35.0f;
                    // Blend Green -> Light Turquoise
                    c.r = (unsigned char)(80 + blend * (60 - 80));
                    c.g = (unsigned char)(175 + blend * (220 - 175));
                    c.b = (unsigned char)(85 + blend * (210 - 85));
                }
            } 
            else if (y > BOTTOM + 160) {
                // LAPISAN TANAH (Soil)
                c = (Color){130, 85, 45, 255};
                
                // Agar tidak kaku, beri sedikit gradasi gelap di pinggiran tanah coklat
                int distToShore = shoreLimit - x;
                if (distToShore < 15) {
                    float darkBlend = (float)(15 - distToShore) / 15.0f;
                    c.r = (unsigned char)(c.r * (1.0f - darkBlend * 0.3f));
                    c.g = (unsigned char)(c.g * (1.0f - darkBlend * 0.3f));
                    c.b = (unsigned char)(c.b * (1.0f - darkBlend * 0.3f));
                }
            } 
            else {
                // LAPISAN AKUIFER (Blue Groundwater)
                c = (Color){50, 140, 210, 255};
            }

            Wrapper_DrawLineThick(x, y, x + 3, y, 2, c);
        }
    }
}