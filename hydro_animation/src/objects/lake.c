#include "../objects/lake.h"
#include "../wrapper/draw_ellipse.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

// =========================================================
// LAKE MODULE - Danau Organik + Aliran Sungai ke Laut
// =========================================================


// =========================================================
// BAGIAN 1: DANAU ORGANIK
// =========================================================

typedef struct {
    int dx;   // offset X dari LAKE_CENTER_X
    int dy;   // offset Y dari LAKE_CENTER_Y
    int rx;   // jari-jari horizontal
    int ry;   // jari-jari vertikal
} EllipsPart;

// Semua rx/ry diperbesar proporsional mengikuti radius baru (110, 50)
static const EllipsPart LAKE_PARTS[] = {
//   dx    dy    rx    ry
    {  0,   0,  110,  46 },   // badan utama
    {-62,  12,   62,  28 },   // tonjolan kiri
    { 60,  -7,   60,  25 },   // tonjolan kanan
    {-12, -22,   78,  25 },   // tonjolan atas
    { 15,  22,   70,  22 },   // tonjolan bawah
};

#define LAKE_PARTS_COUNT 5

static const Color COLOR_LAKE_FILL   = {  80, 155, 225, 200 };
static const Color COLOR_LAKE_SHADOW = {  55, 115, 190, 160 };
static const Color COLOR_LAKE_SHINE  = { 170, 220, 255, 110 };

static void DrawLakeBody() {

    // Pass 1: shadow (kesan kedalaman)
    for (int i = 0; i < LAKE_PARTS_COUNT; i++) {
        int cx = LAKE_CENTER_X + LAKE_PARTS[i].dx;
        int cy = LAKE_CENTER_Y + LAKE_PARTS[i].dy - 4;
        Wrapper_DrawEllipseFilled(cx, cy,
                                  LAKE_PARTS[i].rx + 4,
                                  LAKE_PARTS[i].ry + 3,
                                  COLOR_LAKE_SHADOW);
    }

    // Pass 2: badan utama
    for (int i = 0; i < LAKE_PARTS_COUNT; i++) {
        int cx = LAKE_CENTER_X + LAKE_PARTS[i].dx;
        int cy = LAKE_CENTER_Y + LAKE_PARTS[i].dy;
        Wrapper_DrawEllipseFilled(cx, cy,
                                  LAKE_PARTS[i].rx,
                                  LAKE_PARTS[i].ry,
                                  COLOR_LAKE_FILL);
    }

    // Pass 3: highlight permukaan
    Wrapper_DrawEllipseFilled(LAKE_CENTER_X - 22,
                              LAKE_CENTER_Y + 15,
                              42, 13,
                              COLOR_LAKE_SHINE);
}


// =========================================================
// BAGIAN 2: ALIRAN SUNGAI (DANAU → LAUT)
//
// Titik [0] = sisi kanan danau:
//   LAKE_CENTER_X + LAKE_RADIUS_X = (LEFT+370) + 110 = LEFT+480
//   Y = LAKE_CENTER_Y             = BOTTOM+260
//
// Dari sana sungai mengalir ke kanan sambil turun
// mengikuti gravitasi menuju laut (tepi kanan layar).
// =========================================================

typedef struct { int x; int y; } Point2D;

static const Point2D RIVER_PATH[] = {
    // [0] keluar dari sisi kanan danau
    { LEFT + 480,  BOTTOM + 255 },

    // [1] turun sedikit, belok kanan
    { LEFT + 545,  BOTTOM + 238 },

    // [2] lekukan ke bawah (natural)
    { LEFT + 615,  BOTTOM + 218 },

    // [3] lekukan ke atas sedikit
    { LEFT + 695,  BOTTOM + 205 },

    // [4] turun lagi mengikuti kontur
    { LEFT + 775,  BOTTOM + 190 },

    // [5] mulai mendekati laut
    { LEFT + 860,  BOTTOM + 175 },

    // [6] hampir sampai tepi
    { LEFT + 940,  BOTTOM + 165 },

    // [7] sampai laut (tepi kanan layar)
    { RIGHT,       BOTTOM + 155 },
};

#define RIVER_POINT_COUNT 8

static const Color COLOR_RIVER_EDGE = { 50,  110, 190, 130 };
static const Color COLOR_RIVER_CORE = { 80,  155, 225, 200 };

static void DrawRiverSegment(Point2D a, Point2D b) {
    Wrapper_DrawLineThick(a.x, a.y, b.x, b.y, 8, COLOR_RIVER_EDGE);
    Wrapper_DrawLineThick(a.x, a.y, b.x, b.y, 5, COLOR_RIVER_CORE);
}

static void DrawRiver() {
    for (int i = 0; i < RIVER_POINT_COUNT - 1; i++) {
        DrawRiverSegment(RIVER_PATH[i], RIVER_PATH[i + 1]);
    }
}


// =========================================================
// INTERFACE PUBLIK
// =========================================================

void InitLake() {
    // Danau statis — tidak ada state yang perlu diinisialisasi.
}

void DrawLake() {
    DrawRiver();      // sungai di layer bawah
    DrawLakeBody();   // danau menutupi pangkal sungai
}