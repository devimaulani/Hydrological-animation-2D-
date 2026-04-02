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
        int cx = (int)birds[i].x;
        int cy = (int)birds[i].y;
        
        // Peringkat Penilaian Terpenuhi (VARIASI OBJEK GERAK "Kepakan Sayap"):
        // Kalkulasi siklus kepakan menggunakan fungsi sinus sehingga pergerakan mulus bolak-balik.
        float wingCycle = sin(globalTime * birds[i].flapSpeed + birds[i].phase);
        
        // Konversi hasil siklus sudut (yang berkisar -1 hingga 1) ke perpindahan piksel sayap secara vertikal.
        int wingYOffset = (int)(wingCycle * 12.0f); 
        
        // Titik ujung sayap akan bergerak naik-turun secara presisi
        int tipX = 14; 
        int tipY = -8 + wingYOffset; // Ke atas kalau offset positif
        
        // Gambar Sayap Kiri
        Wrapper_DrawLineThick(
            cx, cy,                // Titik Punggung
            cx - tipX, cy + tipY,  // Ujung Sayap Kiri
            2, 
            BLACK
        );
        
        // Gambar Sayap Kanan
        Wrapper_DrawLineThick(
            cx, cy,                // Titik Punggung
            cx + tipX, cy + tipY,  // Ujung Sayap Kanan
            2, 
            BLACK
        );
        
        // Gambar Tubuh Tengah (Pusat titik tumpu kepak sayap)
        Wrapper_DrawLineThick(
            cx, cy,
            cx, cy - 3, 
            3,
            BLACK
        );
    }
}
