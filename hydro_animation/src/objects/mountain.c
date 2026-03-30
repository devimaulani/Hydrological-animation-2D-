#include "mountain.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

// gunung fleksibel & solid
void DrawMountain() {

    // =========================
    // PARAMETER DINAMIS
    // =========================
    int landTop = -HALF_H / 4;      // permukaan tanah
    int peakY   = landTop + 150;    // tinggi gunung

    // =========================
    // GUNUNG TENGAH
    // =========================
    for (int y = peakY; y >= landTop; y--) {

        int offset = (peakY - y);

        int left  = -250 - offset;
        int right = -250 + offset;

        Wrapper_DrawLineThick(
            left, y,
            right, y,
            2,
            (Color){120, 180, 120, 255}
        );
    }

    // =========================
    // GUNUNG KIRI
    // =========================
    for (int y = peakY - 30; y >= landTop; y--) {

        int offset = (peakY - 30 - y);

        int left  = -380 - offset;
        int right = -380 + offset;

        Wrapper_DrawLineThick(
            left, y,
            right, y,
            2,
            (Color){100, 160, 100, 255}
        );
    }

    // =========================
    // GUNUNG KANAN (TIDAK MASUK LAUT)
    // =========================
    for (int y = peakY - 30; y >= landTop; y--) {

        int offset = (peakY - 30 - y);

        int left  = -120 - offset;
        int right = -120 + offset;

        // batasi agar tidak ke laut
        if (right > 0) right = 0;

        Wrapper_DrawLineThick(
            left, y,
            right, y,
            2,
            (Color){100, 160, 100, 255}
        );
    }
}