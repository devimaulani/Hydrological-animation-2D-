#include "draw_rectangle.h"
#include "../../src/algo/bresenham.h"
#include "../../src/algo/midcircle.h"
#include "../simulation/simulation_state.h"

void Wrapper_DrawRectangleOutline(int x, int y, int width, int height, int thickness, Color color) {
    if (width <= 0 || height <= 0) return;
    if (isWireframeMode && applyWireframeColor) { color.r = 0; color.g = 255; color.b = 0; }
    
    // Top
    Bres_ThickLine(x, y, x + width, y, thickness, color);
    // Bottom
    Bres_ThickLine(x, y + height, x + width, y + height, thickness, color);
    // Left
    Bres_ThickLine(x, y, x, y + height, thickness, color);
    // Right
    Bres_ThickLine(x + width, y, x + width, y + height, thickness, color);
}

void Wrapper_DrawRectangleFilled(int x, int y, int width, int height, Color color) {
    if (width <= 0 || height <= 0) return;
    
    if (isWireframeMode) {
        Wrapper_DrawRectangleOutline(x, y, width, height, 1, color);
        return;
    }

    // FAST SCANLINE: Linear horizontal fill (Much faster than Bresenham for axis-aligned)
    for (int j = 0; j < height; j++) {
        int py = y + j;
        for (int i = 0; i < width; i++) {
            DrawPixel(x + i, py, color);
        }
    }
}

void Wrapper_DrawRectangleRounded(int x, int y, int width, int height, int radius, Color color) {
    if (width <= 0 || height <= 0) return;
    if (radius <= 0) {
        Wrapper_DrawRectangleFilled(x, y, width, height, color);
        return;
    }

    if (isWireframeMode) {
        Wrapper_DrawRectangleOutline(x, y, width, height, 1, color);
        // Rounded wireframe is hard, just use basic outline for wireframe
        return;
    }

    // Radius tidak boleh lebih dari setengah dimensi terkecil
    if (radius > width / 2) radius = width / 2;
    if (radius > height / 2) radius = height / 2;

    // 1. Gambar badan tengah (tanpa sudut)
    Wrapper_DrawRectangleFilled(x + radius, y, width - 2 * radius, height, color);
    Wrapper_DrawRectangleFilled(x, y + radius, width, height - 2 * radius, color);

    // 2. Gambar 4 Sudut (Arc menggunakan Mid-Point logic sederhana via DrawPixel)
    // Sudut Kiri Atas
    for (int ix = 0; ix <= radius; ix++) {
        for (int iy = 0; iy <= radius; iy++) {
            if (ix*ix + iy*iy <= radius*radius) {
                DrawPixel(x + radius - ix, y + radius - iy, color);
            }
        }
    }
    // Sudut Kanan Atas
    for (int ix = 0; ix <= radius; ix++) {
        for (int iy = 0; iy <= radius; iy++) {
            if (ix*ix + iy*iy <= radius*radius) {
                DrawPixel(x + width - radius + ix, y + radius - iy, color);
            }
        }
    }
    // Sudut Kiri Bawah
    for (int ix = 0; ix <= radius; ix++) {
        for (int iy = 0; iy <= radius; iy++) {
            if (ix*ix + iy*iy <= radius*radius) {
                DrawPixel(x + radius - ix, y + height - radius + iy, color);
            }
        }
    }
    // Sudut Kanan Bawah
    for (int ix = 0; ix <= radius; ix++) {
        for (int iy = 0; iy <= radius; iy++) {
            if (ix*ix + iy*iy <= radius*radius) {
                DrawPixel(x + width - radius + ix, y + height - radius + iy, color);
            }
        }
    }
}
