#include "water.h"
#include "coords.h"
#include "../wrapper/draw_line.h"
#include "../wrapper/draw_circle.h"
#include <math.h>
#include "raylib.h"
#include "../simulation/simulation_state.h"

void DrawWater(int yStart) {
    float tOff = (float)GetTime() * 1.5f; 
    int landTop = (int)(-HALF_H / 4.2f); 

    // Langkah vertikal diperkecil (y--) agar air terlihat padat dan halus
    for (int y = yStart; y > BOTTOM; y--) { 
        int depthFactor = yStart - y; 
        int blue = 255 - (depthFactor / 2);
        if (blue < 100) blue = 100;
        int green = 144 - (depthFactor / 3);
        if (green < 50) green = 50;

        Color layerColor = (Color){20, green, blue, 255};

        float tRatio = (float)(landTop - y) / (float)(landTop - BOTTOM); 
        float startX = 100.0f + (100.0f * tRatio);
        startX += (25.0f * sinf((float)y * 0.08f)) + (10.0f * cosf((float)y * 0.2f));
        
        // GESER KE KIRI sedikit agar tidak ada celah dengan land
        startX -= 5.0f;
        
        float amp = 7.0f - (depthFactor * 0.02f);
        if (amp < 0.5f) amp = 0.5f; 
        
        float freq = 0.015f;
        float currentPhase = tOff + ((float)y * 0.1f);
        
        float prevX = startX;
        float prevY = (float)y + (amp * sinf(startX * freq + currentPhase));

        // Langkah horizontal diperkecil (x += 1.0f) agar kurva gelombang mulus
        if (isWireframeMode) {
            // Draw sparse lines or just top boundary
            if (y >= yStart - 2 || y % 50 == 0) {
                Wrapper_DrawLineThick(startX, prevY, RIGHT + 10.0f, prevY, 1.5f, layerColor);
            }
        } else {
            for (float x = startX + 1.0f; x <= (float)RIGHT + 10.0f; x += 1.0f) {
                float bA = x * freq + currentPhase;
                float currentY = (float)y + (amp * sinf(bA));
                // Ketebalan dikurangi agar antar lapisan menyatu halus
                Wrapper_DrawLineThick(prevX, prevY, x, currentY, 1.5f, layerColor);
                prevX = x;
                prevY = currentY;
            }
        }

        // BUIH DEBURAN OMBAK (SHORELINE FOAM)
        // Digambar di titik pertemuan air dan tanah (startX)
        float foamSway = 4.0f * sinf((float)GetTime() * 6.0f + (float)y * 0.15f);
        float alphaOsc = 150 + 100 * sinf((float)GetTime() * 8.0f + (float)y * 0.5f);
        Color foamCol = (Color){220, 245, 255, (unsigned char)alphaOsc};
        
        // Gambar beberapa lingkaran busa kecil yang bertumpuk (cluster)
        // Mengecil seiring kedalaman (y)
        float foamSize = 4.0f * (1.0f - (float)depthFactor / (yStart - BOTTOM));
        if (foamSize < 1.0f) foamSize = 1.0f; 

        Wrapper_DrawCircleFilled(startX + foamSway, (float)y, foamSize, foamCol);
        if (y % 3 == 0) {
            Wrapper_DrawCircleFilled(startX + foamSway - 3.0f, (float)y + 2.0f, foamSize * 0.7f, foamCol);
        }
    }
    
    // Perbaikan Buih di permukaan air (Shoreline foam)
    float tRatioBuih = (float)(landTop - yStart) / (float)(landTop - BOTTOM); 
    float buihStartX = 100.0f + (100.0f * tRatioBuih);
    buihStartX += (25.0f * sinf((float)yStart * 0.08f)) + (10.0f * cosf((float)yStart * 0.2f)) - 5.0f;
    
    float pX = buihStartX;
    float buihPhase = tOff + ((float)yStart * 0.1f);
    float pY = (float)yStart + (7.0f * sinf(buihStartX * 0.015f + buihPhase));
    
    // Resolusi buih ditingkatkan
    for (float x = buihStartX + 2.0f; x <= (float)RIGHT + 10.0f; x += 2.0f) {
        float bA = x * 0.015f + buihPhase;
        float cY = (float)yStart + (7.0f * sinf(bA));
        Wrapper_DrawLineThick(pX, pY + 1.5f, x, cY + 1.5f, 2.0f, (Color){180, 230, 255, 180});
        pX = x;
        pY = cY;
    }
}
