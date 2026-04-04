#include "infiltration.h"
#include "raylib.h"
#include "../objects/lake.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_ellipse.h"
#include "../objects/land.h"
#include "coords.h"
#include "../simulation/simulation_state.h"
#include <stdlib.h>
#include <math.h>

// =========================================================
// INFILTRATION MODULE - Jalur Aliran Resapan Berakar
// Menyelesaikan masalah "kotak-kotak" dengan menggambar lintasan utuh
// yang terus terlihat namun warna / intensitas alirannya berdenyut turun.
// =========================================================

#define VEIN_COUNT 12
#define VEIN_SEGMENTS 8

typedef struct {
    float x, y;
} Node;

typedef struct {
    Node nodes[VEIN_SEGMENTS];
    float speed;
    float x; // Base X
} Vein;

static Vein veins[VEIN_COUNT];
static float soilTop, aquiferTopDefault;

// Mendapatkan X acak di area sungai
static float RandomRiverX() {
    return (float)(LEFT + 250 + (rand() % 400));
}

void InitInfiltration() {
    soilTop = BOTTOM + 155; 
    aquiferTopDefault = BOTTOM + 130; 

    for (int v = 0; v < VEIN_COUNT; v++) {
        float cx = RandomRiverX();
        veins[v].x = cx;
        
        // --- DINAMIS: Ikuti Gelombang Aqifer ---
        float wave = (float)GetLayerWave((int)cx);
        float currentAquiferTop = aquiferTopDefault + wave;
        float currentSoilTop = soilTop + wave;

        veins[v].nodes[0].x = cx;
        veins[v].nodes[0].y = currentSoilTop;
        
        float dy = (currentSoilTop - currentAquiferTop) / (float)VEIN_SEGMENTS;
        
        for (int i = 1; i < VEIN_SEGMENTS; i++) {
            veins[v].nodes[i].x = cx + (float)((rand() % 20) - 10);
            veins[v].nodes[i].y = currentSoilTop - (dy * (float)i);
        }
    }
}


void UpdateInfiltration(float dt) {
    // Jalurnya statis permanen bagai pembuluh, animasinya terjadi secara visual di fungsi Draw
}

void DrawInfiltration() {
    float time = (float)GetTime();

    for (int v = 0; v < VEIN_COUNT; v++) {
        for (int i = 0; i < VEIN_SEGMENTS - 1; i++) {
            float phase = (time * 5.0f) - (i * 0.35f);
            float pulse = (sinf(phase) + 1.0f) / 2.0f; 
            
            int baseAlpha = 220 - (i * 4);
            if (baseAlpha < 20) baseAlpha = 20;

            Color c = (Color){50, 110, 170, (unsigned char)(baseAlpha * 0.3f)};
            float thickness = 2.0f;
            
            if (pulse > 0.75f) {
                c = (Color){80, 180, 255, (unsigned char)baseAlpha}; 
                thickness = 4.0f;
            }
            
            Wrapper_DrawLineThick(
                veins[v].nodes[i].x, veins[v].nodes[i].y, 
                veins[v].nodes[i+1].x, veins[v].nodes[i+1].y, 
                thickness, c
            );

            if (i == VEIN_SEGMENTS - 2 && pulse > 0.4f) {
                float intensity = pulse - 0.4f;
                Wrapper_DrawEllipseFilled(
                    veins[v].nodes[i+1].x, 
                    veins[v].nodes[i+1].y, 
                    30.0f * intensity,
                    6.0f * intensity,
                    c
                );
            }
        }
    }
}