#include "cloud.h"
#include "../wrapper/draw_ellipse.h"
#include <math.h>

// Membantu pergerakan awan horizontal lembut berkat waktu (internal swaying)
static float GetInternalSway(float multiplier, float offsetPhase) {
    float time = (float)GetTime();
    return sinf(time * 0.4f + offsetPhase) * 15.0f * multiplier;
}

// Helper: Lerp satu byte warna
static unsigned char LerpByte(unsigned char a, unsigned char b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (unsigned char)((float)a + ((float)b - (float)a) * t);
}

// 1. Awan Tipe A (Standard - Growing Size)
void DrawCloud(float x, float y, float opacity) {
    unsigned char alpha = (unsigned char)(255 * opacity);
    if (alpha < 5) alpha = 0; 
    
    Color c = (Color){245, 245, 245, alpha};
    
    // GROWING SIZE: Awan membesar dari 40% hingga 100% ukurannya
    float growth = 0.4f + (0.6f * opacity);
    float baseW = 50.0f * growth;
    float baseH = 25.0f * growth;
    
    float shiftX = x + GetInternalSway(1.0f, 0.0f);

    Wrapper_DrawEllipseFilled(shiftX, y, baseW, baseH, c);
    Wrapper_DrawEllipseFilled(shiftX - 35.0f * growth, y + 15.0f * growth, 40.0f * growth, 22.0f * growth, c);
    Wrapper_DrawEllipseFilled(shiftX + 35.0f * growth, y + 15.0f * growth, 40.0f * growth, 22.0f * growth, c);
    Wrapper_DrawEllipseFilled(shiftX, y + 30.0f * growth, 35.0f * growth, 20.0f * growth, c);
}

// 2. Awan Tipe B (Dynamic - Growing Size & Gradual Dark State)
//    darkProgress: 0.0 = putih murni, 1.0 = gelap penuh (abu-abu tua)
void DrawCloudDynamic(float x, float y, float opacity, float darkProgress) {
    unsigned char alpha = (unsigned char)(255 * opacity);
    if (alpha < 5) alpha = 0;

    // Transisi warna bertahap: putih → abu-abu tua
    Color white = {255, 255, 255, alpha};
    Color dark  = {70, 75, 85, alpha};

    Color cloudColor;
    cloudColor.r = LerpByte(white.r, dark.r, darkProgress);
    cloudColor.g = LerpByte(white.g, dark.g, darkProgress);
    cloudColor.b = LerpByte(white.b, dark.b, darkProgress);
    cloudColor.a = alpha;

    float growth = 0.4f + (0.6f * opacity);
    float baseW = 65.0f * growth;
    float baseH = 28.0f * growth;
    
    float shiftX = x + GetInternalSway(1.2f, 2.5f);

    Wrapper_DrawEllipseFilled(shiftX, y, baseW, baseH, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX - 45.0f * growth, y + 10.0f * growth, 45.0f * growth, 25.0f * growth, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX + 45.0f * growth, y + 10.0f * growth, 45.0f * growth, 25.0f * growth, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX - 20.0f * growth, y + 35.0f * growth, 40.0f * growth, 22.0f * growth, cloudColor);
    Wrapper_DrawEllipseFilled(shiftX + 20.0f * growth, y + 35.0f * growth, 40.0f * growth, 22.0f * growth, cloudColor);
}