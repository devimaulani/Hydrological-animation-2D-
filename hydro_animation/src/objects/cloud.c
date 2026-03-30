#include "cloud.h"
#include "../wrapper/draw_ellipse.h"
#include "../simulation/simulation_state.h"
#include "../animation/advection.h"

// awan fleksibel & sesuai Cartesian
void DrawCloud(int x, int y) {

    Color c = (Color){240,240,240,255};

    int scale = (int)(cloudDensity * 20); // inti kondensasi

    Wrapper_DrawEllipseFilled(x, y, 50+scale, 25+scale/2, c);

    Wrapper_DrawEllipseFilled(x-30, y+20, 35+scale/2, 20+scale/3, c);
    Wrapper_DrawEllipseFilled(x+30, y+20, 35+scale/2, 20+scale/3, c);

    Wrapper_DrawEllipseFilled(x, y+35, 30+scale/2, 18+scale/3, c);
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

    Wrapper_DrawEllipseFilled(x, y, 50, 25, cloudColor);
    Wrapper_DrawEllipseFilled(x - 30, y + 20, 35, 20, cloudColor);
    Wrapper_DrawEllipseFilled(x + 30, y + 20, 35, 20, cloudColor);
    Wrapper_DrawEllipseFilled(x, y + 35, 30, 18, cloudColor);
}