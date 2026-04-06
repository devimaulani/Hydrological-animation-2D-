#include "logo_polban.h"
#include "../wrapper/draw_line.h"
#include "../simulation/simulation_state.h"
#include "coords.h"
#include <math.h>

void DrawLogoPolban(float cx, float cy, float scale) {
    float R = 40.0f * scale;
    float W = 0.866025f * R; // Half width (sqrt(3)/2 * R)
    
    Color orange = (Color){255, 140, 0, 255};
    Color blue   = (Color){20, 20, 160, 255};
    Color wireColor = (Color){0, 255, 0, 255};

    float rInnerSq = (0.45f * R)*(0.45f * R); // Radius dalam sedikit lebih kecil dari 0.5R untuk proporsi

    // Scanline horizontal untuk mengisi/meng-outline logo
    // Step 0.5f agar rapat jika solid fill
    float step = isWireframeMode ? 1.0f : 0.5f;

    for (float dy = -R; dy <= R; dy += step) {
        float minX, maxX;
        // Hexagon bounds calculation
        if (dy > 0.5f * R) {
            float t = (R - dy) / (0.5f * R); 
            maxX = W * t;
        } else if (dy < -0.5f * R) {
            float t = (dy + R) / (0.5f * R); 
            maxX = W * t;
        } else {
            maxX = W;
        }
        minX = -maxX;

        float currentSegStartX = -9999;
        Color currentSegColor = BLANK;

        for (float dx = minX; dx <= maxX + 1.0f; dx += step) {
            Color c = BLANK;
            
            // Cek apakah dx masih di dalam batas Hexagon
            if (dx <= maxX + 0.1f) {
                float rSq = dx*dx + dy*dy;
                
                // Jika di luar lingkaran putih pusat
                if (rSq >= rInnerSq) {
                    float dist = dy - 0.57735f * dx; // 30 degree slope gradient
                    float normDist = dist / R;
                    
                    if (normDist > 0.05f) { // Bagian Kiri Atas (Orange)
                        // Tiga garis / stripes Orange
                        if ((normDist >= 0.05f && normDist <= 0.28f) ||
                            (normDist >= 0.36f && normDist <= 0.59f) ||
                            (normDist >= 0.67f && normDist <= 0.90f)) {
                            c = orange;
                        }
                    } else if (normDist < -0.05f) { // Bagian Kanan Bawah (Blue)
                        c = blue;
                    }
                }
            }

            // Jika dalam mode wireframe, identifikasi batas (edge detection)
            if (isWireframeMode && c.a > 0) {
                bool isEdge = false;
                // Batas luar hexagon
                if (dy <= -R + 2.0f || dy >= R - 2.0f || dx <= minX + 1.5f || dx >= maxX - 1.5f) isEdge = true;
                
                // Batas lingkaran dalam
                float rSq = dx*dx + dy*dy;
                if (rSq >= rInnerSq && rSq <= rInnerSq + 2.0f*R) isEdge = true;
                
                // Batas garis stripe (diagonal)
                float dist = dy - 0.57735f * dx;
                float normDist = dist / R;
                float b[7] = {-0.05f, 0.05f, 0.28f, 0.36f, 0.59f, 0.67f, 0.90f};
                for(int i=0; i<7; i++) {
                    if (fabsf(normDist - b[i]) < 0.025f) isEdge = true;
                }
                
                if (!isEdge) {
                    c = BLANK; // Kosongkan bagian dalam jika wireframe
                } else if (applyWireframeColor) {
                    c = wireColor;
                }
            }

            // Stateful accumulation untuk menggambar Line Segment per baris (Optimasi)
            bool colorChanged = (c.r != currentSegColor.r || c.g != currentSegColor.g || c.b != currentSegColor.b || c.a != currentSegColor.a);
            
            if (colorChanged || dx > maxX) {
                // Gambar segmen sebelumnya jika ada warnanya
                if (currentSegColor.a > 0 && currentSegStartX > -9998) {
                    float px1 = cx + currentSegStartX;
                    float px2 = cx + dx - step;
                    float py = cy + dy;
                    
                    if (px2 >= px1) {
                        if (isWireframeMode) {
                            // Di wireframe mode menggambar pinggiran (vertex loop outline)
                            // Jika px1 == px2 maka cukup dot saja
                            Wrapper_DrawLineThick(px1, py, px2 + 0.1f, py, 1.5f, currentSegColor);
                        } else {
                            // Fill utuh horizontal segment
                            Wrapper_DrawLine(px1, py, px2 + 0.5f, py, currentSegColor);
                        }
                    }
                }
                // Mulai segmen baru
                currentSegStartX = dx;
                currentSegColor = c;
            }
        }
    }
}