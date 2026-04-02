#include "tree.h"
#include "../wrapper/draw_circle.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

// ==========================
// BATANG
// ==========================
void DrawTrunk(int x, int y) {

    int height = 50; // Tinggi dinaikkan sedikit agar proporsional akar-ke-daun

    // Batang tidak hanya 1 garis lurus, tapi diberi sedikit ketebalan bertingkat
    Wrapper_DrawLineThick(
        x, y,
        x, y + height,
        4,
        (Color){100, 60, 30, 255} // Coklat tua
    );
    // Highlight batang
    Wrapper_DrawLineThick(
        x - 1, y + 5,
        x - 1, y + height - 5,
        2,
        (Color){130, 80, 40, 255} // Coklat muda
    );
}

// ==========================
// DAUN / KANOPI POHON
// ==========================
void DrawLeaves(int x, int y) {

    int offset = 55; // Titik pusat tajuk daun

    // Menggambar lapisan daun yang tumpang tindih untuk kesan rimbun alami
    // Lapis bayangan (belakang, hijau gelap)
    Wrapper_DrawCircleFilled(x - 20, y + offset + 5, 20, (Color){20, 100, 30, 255});
    Wrapper_DrawCircleFilled(x + 18, y + offset - 2, 22, (Color){15, 90, 25, 255});
    
    // Lapis tengah (hijau daun normal)
    Wrapper_DrawCircleFilled(x - 8, y + offset - 10, 25, (Color){30, 140, 40, 255});
    Wrapper_DrawCircleFilled(x + 10, y + offset + 12, 23, (Color){34, 150, 45, 255});
    Wrapper_DrawCircleFilled(x - 15, y + offset - 5, 18, (Color){28, 130, 38, 255});
    Wrapper_DrawCircleFilled(x + 22, y + offset + 8, 16, (Color){25, 120, 35, 255});

    // Lapis highlight (atas/tepi, hijau terang merespons datangnya sinar mentari)
    Wrapper_DrawCircleFilled(x + 8, y + offset + 20, 15, (Color){50, 180, 60, 255});
    Wrapper_DrawCircleFilled(x - 5, y + offset + 15, 13, (Color){60, 190, 70, 255});
}

// ==========================
// POHON
// ==========================
void DrawTree(int x, int y) {
    DrawTrunk(x, y);
    DrawLeaves(x, y);
}