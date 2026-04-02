#include "cloud.h"
#include "../wrapper/draw_ellipse.h"
#include "../simulation/simulation_state.h"
#include "../animation/advection.h"
#include <math.h>
#include "raylib.h"

// Membantu pergerakan awan horizontal berulang berkat waktu
static int GetWindOffset(float multiplier, float offsetPhase) {
    float time = GetTime();
    // Awan akan bergeser ke kiri dan kanan sedikit demi sedikit, atau mengambang
    return (int)(sin(time * 0.2f + offsetPhase) * 40.0f * multiplier);
}

// awan fleksibel & sesuai Cartesian
void DrawCloud(int x, int y) {

    Color c = (Color){240,240,240,255};

    int scale = (int)(cloudDensity * 20); // inti kondensasi
    
    // Angin menggeser posisi asli
    int shiftX = x + GetWindOffset(1.0f, 0.0f);

    Wrapper_DrawEllipseFilled(shiftX, y, 50+scale, 25+scale/2, c);

    Wrapper_DrawEllipseFilled(shiftX-30, y+20, 35+scale/2, 20+scale/3, c);
    Wrapper_DrawEllipseFilled(shiftX+30, y+20, 35+scale/2, 20+scale/3, c);

    Wrapper_DrawEllipseFilled(shiftX, y+35, 30+scale/2, 18+scale/3, c);
}

void DrawCloudDynamic(int x, int y) {

    // 🔥 cek kondisi angin
    int isDark = IsWindReachedLand();

    Color cloudColor;

    if (isDark) {
        cloudColor = (Color){120, 120, 120, 255}; // gelap (mendung)
    } else {
        cloudColor = (Color){240, 240, 240, 255}; // normal
    }

    // Angin menggeser posisi asli dengan fase berbeda
    int shiftX = x + GetWindOffset(1.5f, 2.0f);

    Wrapper_DrawEllipseFilled(shiftX, y, 50, 25, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX - 30, y + 20, 35, 20, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX + 30, y + 20, 35, 20, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX, y + 35, 30, 18, cloudColor);
}