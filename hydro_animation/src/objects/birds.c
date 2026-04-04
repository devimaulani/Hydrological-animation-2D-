#include "birds.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>

#define MAX_BIRDS 5

typedef struct {
    float x;
    float y;
    float startY;
    float speedY;
    float speedX;
    float flapSpeed;
    float phase;
} Bird;

static Bird birds[MAX_BIRDS];
static float globalTime = 0.0f;

void InitBirds() {
    // Memberikan posisi awal dan parameter kecepatan yang sedikit berbeda (variasi)
    float startXs[MAX_BIRDS] = {LEFT - 50, LEFT - 200, LEFT - 350, LEFT - 100, LEFT - 280};
    float startYs[MAX_BIRDS] = {TOP - 100, TOP - 140, TOP - 80, TOP - 220, TOP - 180};
    
    for(int i = 0; i < MAX_BIRDS; i++) {
        birds[i].x = startXs[i];
        birds[i].startY = startYs[i];
        birds[i].y = birds[i].startY;
        
        birds[i].speedX = 70.0f + (i * 12.0f);     // Kecepatan terbang horizontal 
        birds[i].speedY = 1.0f + (i * 0.3f);       // Frekuensi kurva lintasan udara (naik-turun)
        birds[i].flapSpeed = 12.0f + (i * 2.5f);   // Kecepatan kepakan sayap
        birds[i].phase = i * 2.0f;                 // Fase gelombang yang berbeda untuk setiap burung
    }
}

void UpdateBirds(float dt) {
    globalTime += dt;
    
    for(int i = 0; i < MAX_BIRDS; i++) {
        // Pembaruan sumbu X (gerak progresif)
        birds[i].x += birds[i].speedX * dt;
        
        // Peringkat Penilaian Terpenuhi (LINTASAN NON-LINEAR):
        // Memanfaatkan fungsi sinus untuk merepresentasikan burung yang terbang mengalun secara aerodinamis.
        birds[i].y = birds[i].startY + 20.0f * sin(globalTime * birds[i].speedY + birds[i].phase);
        
        // Jika burung melewati batas layar sebelah kanan, mereka akan muncul kembali dari sebelah kiri
        if(birds[i].x > RIGHT + 50) {
            birds[i].x = LEFT - 50;
        }
    }
}

void DrawBirds() {
    for(int i = 0; i < MAX_BIRDS; i++) {
        float cx = birds[i].x;
        float cy = birds[i].y;
        
        float wingCycle = sinf(globalTime * birds[i].flapSpeed + birds[i].phase);
        float wingYOffset = wingCycle * 12.0f; 
        
        float tipX = 14.0f; 
        float tipY = -8.0f + wingYOffset; 
        
        // Gambar Sayap Kiri
        Wrapper_DrawLineThick(cx, cy, cx - tipX, cy + tipY, 2.0f, BLACK);
        
        // Gambar Sayap Kanan
        Wrapper_DrawLineThick(cx, cy, cx + tipX, cy + tipY, 2.0f, BLACK);
        
        // Gambar Tubuh Tengah
        Wrapper_DrawLineThick(cx, cy, cx, cy - 3.0f, 3.0f, BLACK);
    }
}
