#include "tree.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_ellipse.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>
#include <stdlib.h>

// ==========================
// PALET WARNA REALISTIS
// ==========================
static const Color TRUNK_DARK    = { 65,  40,  25,  255 }; // Bark Gelap
static const Color TRUNK_NORMAL  = { 90,  55,  35,  255 }; // Bark Standar
static const Color TRUNK_LIGHT   = { 120, 85,  55,  255 }; // Highlight Bark

static const Color LEAF_DEEP     = { 10,  60,  15,  255 }; // Shadow foliage
static const Color LEAF_NORMAL   = { 40,  135, 40,  255 }; // Mid foliage
static const Color LEAF_BRIGHT   = { 85,  185, 80,  255 }; // Sun-lit foliage


// ==========================
// HELPER: TEKSTUR DAUN ORGANIK (RAPID CLOUD)
// Menggunakan lingkaran yang lebih besar dan teratur untuk kesan "puffy"
// agar tidak terlihat berantakan namun tetap rimbun.
// ==========================
static void DrawOrganicLeafCloud(int cx, int cy, int rx, int ry, int density) {
    // 1. Layer Bayangan Dasar (Lebih besar untuk pondasi rimbun)
    for (int i = 0; i < density / 2; i++) {
        int ox = (rand() % (rx * 2)) - rx;
        int oy = (rand() % (ry * 2)) - ry;
        int r  = 12 + (rand() % 10); // Lingkaran lebih besar agar rapi
        Wrapper_DrawCircleFilled(cx + ox, cy + oy, r, LEAF_DEEP);
    }
    
    // 2. Layer Tengah (Warna Normal - Membentuk volume)
    for (int i = 0; i < density; i++) {
        int ox = (rand() % (int)(rx * 1.4f)) - (int)(rx * 0.7f);
        int oy = (rand() % (int)(ry * 1.4f)) - (int)(ry * 0.7f);
        int r  = 8 + (rand() % 8);
        Wrapper_DrawCircleFilled(cx + ox, cy + oy, r, LEAF_NORMAL);
    }

    // 3. Layer Atas (Highlight - Memberikan detail rapi)
    for (int i = 0; i < density / 2; i++) {
        int ox = (rand() % (int)(rx * 1.2f)) - (int)(rx * 0.6f);
        int oy = (rand() % (int)(ry * 1.0f)) - (int)(ry * 0.4f);
        int r  = 6 + (rand() % 6);
        Wrapper_DrawCircleFilled(cx + ox, cy + oy, r, LEAF_BRIGHT);
    }
}

// ==========================
// HELPER: BATANG BERTEKSTUR & PADAT (SOLID TAPERED)
// ==========================
static void DrawRealisticTrunk(int x, int y, int h, int startW, int endW) {
    for (int i = 0; i < h; i++) {
        float t = (float)i / (float)h;
        int currentW = startW - (int)(t * (startW - endW));
        
        // Menggunakan ketebalan 4 agar garis saling menindih (solid tanpa celah)
        Wrapper_DrawLineThick(x - currentW/2, y + i, x + currentW/2, y + i, 4, TRUNK_NORMAL);
        
        // Tekstur kulit kayu tetap dipertahankan dengan kerapatan yang disesuaikan
        if (i % 5 == 0) {
            Wrapper_DrawLineThick(x - currentW/3, y + i, x - currentW/5, y + i, 2, TRUNK_LIGHT);
        }
    }
}


static void DrawSolidBranch(float x1, float y1, float x2, float y2, float thick, Color col) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float len = sqrtf(dx*dx + dy*dy);
    if (len < 0.1f) return;

    // Gunakan radius setengah dari thickness
    float radius = thick / 2.0f;
    
    // Iterasi sepanjang dahan dengan step kecil agar solid
    // Gunakan step 0.5 agar lingkaran saling menindih rapat
    for (float i = 0; i <= len; i += 0.5f) {
        float ratio = i / len;
        float curX = x1 + (dx * ratio);
        float curY = y1 + (dy * ratio);
        
        // Gambar lingkaran pengisi (brush)
        Wrapper_DrawCircleFilled(curX, curY, radius, col);
    }
}

// ==========================
// POHON TINGGI (ANALISIS GAMBAR 1)
// Karakteristik: Batang meruncing tinggi, cabang melengkung, kluster daun terfragmentasi
// ==========================
void DrawTreeTall(int x, int y) {
    // 1. Batang Pokok (Tapered: 14px -> 6px)
    DrawRealisticTrunk(x, y, 70, 14, 6);
    
    // 2. Cabang-cabang Padat (Solid Branching)
    DrawSolidBranch(x, y + 45, x - 30, y + 65, 7, TRUNK_DARK);
    DrawSolidBranch(x - 30, y + 65, x - 45, y + 80, 5, TRUNK_NORMAL);
    DrawSolidBranch(x, y + 60, x + 25, y + 80, 6, TRUNK_DARK);
    DrawSolidBranch(x, y + 70, x + 5, y + 95, 5, TRUNK_NORMAL);

    // 3. Kluster Daun Prosedural (Organik)
    DrawOrganicLeafCloud(x - 45, y + 85, 25, 20, 25); // Kluster bawah kiri
    DrawOrganicLeafCloud(x + 30, y + 85, 22, 18, 20); // Kluster kanan
    DrawOrganicLeafCloud(x + 5, y + 105, 28, 22, 30); // Kluster puncak
}

// ==========================
// POHON RIMBUN (ANALISIS GAMBAR 2)
// Karakteristik: Batang ditinggikan sedikit, kanopi diperkecil agar proporsional
// ==========================
void DrawTreeBushy(int x, int y) {
    // 1. Detail Dasar (Semak & Batu)
    srand(x + y);
    for (int i = 0; i < 4; i++) {
        int ox = (rand() % 40) - 20;
        int r = 5 + (rand() % 10);
        Wrapper_DrawCircleFilled(x + ox, y + 5, r, LEAF_DEEP);
    }
    // Batu kecil di akar
    Wrapper_DrawEllipseFilled(x - 12, y + 3, 15, 8, (Color){100, 100, 100, 255});

    // 2. Batang Utama (Dibuat lebih tinggi: 40 -> 55)
    DrawRealisticTrunk(x, y, 55, 20, 12);

    // 3. Kanopi Lebih Rapi (Radius dikurangi: 55 -> 40)
    int canopyY = y + 75;
    DrawOrganicLeafCloud(x, canopyY, 42, 35, 45);       // Pusat
    DrawOrganicLeafCloud(x - 28, canopyY - 10, 25, 20, 25); // Sayap Kiri
    DrawOrganicLeafCloud(x + 28, canopyY - 10, 25, 18, 25); // Sayap Kanan
}

// ==========================
// POHON BESAR/RINDANG (ANALISIS GAMBAR 3)
// Karakteristik: Batang ditinggikan, bentang kanopi dikurangi sedikit
// ==========================
void DrawTreeLarge(int x, int y) {
    // 1. Batang Utama (Dibuat lebih tinggi: 35 -> 50)
    DrawRealisticTrunk(x, y, 50, 30, 18);
    
    // 2. Percabangan Lebar (Solid Spreading) - Diperkecil agar lebih rapi
    DrawSolidBranch(x, y + 50, x - 50, y + 80, 8, TRUNK_DARK);
    DrawSolidBranch(x, y + 50, x + 50, y + 75, 7, TRUNK_DARK);

    // 3. Kanopi Rindang (Dikecilkan sedikit agar jarak ranting jelas)
    int foliY = y + 105;
    DrawOrganicLeafCloud(x - 65, foliY, 35, 30, 35); // Sisi kiri
    DrawOrganicLeafCloud(x + 65, foliY - 10, 35, 28, 30); // Sisi kanan
    DrawOrganicLeafCloud(x, foliY + 15, 50, 35, 50); // Tengah
}


// ==========================
// DEFAULT POHON
// ==========================
void DrawTree(int x, int y) {
    DrawTreeBushy(x, y);
}