#include "water.h"
#include "coords.h"
#include "../wrapper/draw_line.h"
#include <math.h>
#include "raylib.h"

void DrawWater(int yStart) {
    float tOff = GetTime() * 1.5f; // Kecepatan lambat aliran perairan
    int landTop = (int)(-HALF_H / 4.2f); 

    // Laut digambar berlapis perlahan dari atas ke dasar
    for (int y = yStart; y > BOTTOM; y -= 4) { // Turun tiap 4 piksel untuk mengefisiensi performa 
        
        // Gradasi warna: makin bawah makin gelap
        int depthFactor = yStart - y; 
        int blue = 255 - (depthFactor / 2);
        if (blue < 100) blue = 100;
        int green = 144 - (depthFactor / 3);
        if (green < 50) green = 50;

        Color layerColor = (Color){20, green, blue, 255};

        // Menyesuaikan titik mulai laut agar persis menyambung ke tepian daratan
        // Formula HARUS sama dengan GetShoreX di land.c
        float tRatio = (float)(landTop - y) / (float)(landTop - BOTTOM); 
        int startX = 100 + (int)(100.0f * tRatio);
        startX += (int)(25.0f * sinf((float)y * 0.08f)) + (int)(10.0f * cosf((float)y * 0.2f));
        
        // Dinamika ombak di setiap lapisan (makin ke dasar laut, amplitudonya makin kecil/mereda)
        float amp = 7.0f - (depthFactor * 0.02f);
        if (amp < 0.5f) amp = 0.5f; 
        
        float freq = 0.015f;
        
        // Fase ombak bergeser di setiap kedalaman 'y' yang berbeda, sehingga animasi laut tampak bergulung menyatu dari dasar
        float currentPhase = tOff + (y * 0.1f);
        
        int prevX = startX;
        int prevY = y + (int)(amp * sinf(startX * freq + currentPhase));

        // Gambar perlapisan gelombang tebal (ketebalan 6 menutup sela lompatan y-=4 yang hilang)
        for (int x = startX + 10; x <= RIGHT + 10; x += 10) {
            float bA = x * freq + currentPhase;
            int currentY = y + (int)(amp * sinf(bA));
            Wrapper_DrawLineThick(prevX, prevY, x, currentY, 6, layerColor);
            prevX = x;
            prevY = currentY;
        }
    }
    
    // Sentuhan Terakhir: Buih cerah (highlight air) mengambang tegas namun menyatu memeluk garis riak ombak paling atas saja
    float tRatioBuih = (float)(landTop - yStart) / (float)(landTop - BOTTOM); 
    int buihStartX = 100 + (int)(100.0f * tRatioBuih);
    buihStartX += (int)(25.0f * sinf((float)yStart * 0.08f)) + (int)(10.0f * cosf((float)yStart * 0.2f));
    
    int pX = buihStartX;
    float buihPhase = tOff + (yStart * 0.1f);
    int pY = yStart + (int)(7.0f * sinf(buihStartX * 0.015f + buihPhase));
    
    for (int x = buihStartX + 10; x <= RIGHT + 10; x += 10) {
        float bA = x * 0.015f + buihPhase;
        int cY = yStart + (int)(7.0f * sinf(bA));
        Wrapper_DrawLineThick(pX, pY + 2, x, cY + 2, 4, (Color){180, 230, 255, 180});
        pX = x;
        pY = cY;
    }
}
