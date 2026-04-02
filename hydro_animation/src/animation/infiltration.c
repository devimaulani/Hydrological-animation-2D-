#include "infiltration.h"
#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "coords.h"
#include <stdlib.h>
#include <math.h>

// =========================================================
// INFILTRATION MODULE - Jalur Aliran Resapan Berakar
// Menyelesaikan masalah "kotak-kotak" dengan menggambar lintasan utuh
// yang terus terlihat namun warna / intensitas alirannya berdenyut turun.
// =========================================================

#define VEIN_COUNT     6
#define VEIN_SEGMENTS  30

typedef struct {
    float x;
    float y;
} VeinNode;

typedef struct {
    VeinNode nodes[VEIN_SEGMENTS];
} Vein;

static Vein veins[VEIN_COUNT];
static int soilTop;
static int aquiferTop;

static float RandomRiverX() {
    int minX = LEFT + 230;
    int maxX = 50;  // Batasi agar tidak muncul di tepian yang sudah dimundurkan
    int range = maxX - minX;
    return (float)(minX + (rand() % range));
}

void InitInfiltration() {
    soilTop = BOTTOM + 180; 
    // Mengikuti batas aquifer tebal yang Anda tetapkan 
    aquiferTop = BOTTOM + 160; 

    for (int v = 0; v < VEIN_COUNT; v++) {
        float cx = RandomRiverX();
        float cy = (float)soilTop;
        
        float depthRange = (float)(soilTop - aquiferTop);
        float stepY = depthRange / (VEIN_SEGMENTS - 1);
        
        for (int i = 0; i < VEIN_SEGMENTS; i++) {
            veins[v].nodes[i].x = cx;
            veins[v].nodes[i].y = cy;
            
            // Memberikan rute acak/zigzag terbatas agar tetap di dalam tanah
            cx += (float)((rand() % 11) - 5.5f);
            cy -= stepY;
        }
    }
}

void UpdateInfiltration(float dt) {
    // Jalurnya statis permanen bagai pembuluh, animasinya terjadi secara visual di fungsi Draw
}

extern double GetTime(void);

void DrawInfiltration() {
    float time = (float)GetTime();

    for (int v = 0; v < VEIN_COUNT; v++) {
        for (int i = 0; i < VEIN_SEGMENTS - 1; i++) {
            // Animasi denyut (pulse) merambat turun
            float phase = (time * 5.0f) - (i * 0.35f);
            float pulse = (sinf(phase) + 1.0f) / 2.0f; 
            
            // Makin ke bawah makin redup alaminya
            int baseAlpha = 220 - (i * 4);
            if (baseAlpha < 20) baseAlpha = 20;

            // Mode "kering" atau "remang" saat tidak ada tetesan air melewatinya
            Color c = (Color){50, 110, 170, (unsigned char)(baseAlpha * 0.3f)};
            int thickness = 2;
            
            // Mode "basah/dilewati air" saat denyut (pulse) aliran bergerak menimpanya
            if (pulse > 0.75f) {
                c = (Color){80, 180, 255, (unsigned char)baseAlpha}; 
                thickness = 4; // Terlihat lebih nyata dan menonjol
            }
            
            // Gambar urat urat tanah secara bersambung
            Wrapper_DrawLineThick(
                (int)veins[v].nodes[i].x, (int)veins[v].nodes[i].y, 
                (int)veins[v].nodes[i+1].x, (int)veins[v].nodes[i+1].y, 
                thickness, c
            );

            // Pada ujung node (batas aquifer), gambar efek genangan melebar perlahan
            if (i == VEIN_SEGMENTS - 2 && pulse > 0.4f) {
                float intensity = pulse - 0.4f; // 0.0 -> 0.6
                Wrapper_DrawEllipseFilled(
                    (int)veins[v].nodes[i+1].x, 
                    (int)veins[v].nodes[i+1].y, 
                    (int)(30.0f * intensity),   // Melebar ke samping 
                    (int)(6.0f * intensity),   // Sangat tipis membentuk genangan
                    c
                );
            }
        }
    }
}