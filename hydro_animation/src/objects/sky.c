#include "sky.h"
#include "raylib.h"

// Lerp helper untuk unsigned char
static unsigned char LerpByte(unsigned char a, unsigned char b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (unsigned char)((float)a + ((float)b - (float)a) * t);
}

void DrawSkyGradual(float skyDarkness) {
    // Warna langit cerah (SKYBLUE)
    Color bright = {102, 191, 255, 255};
    // Warna langit gelap (mendung / storm)
    Color dark = {30, 35, 55, 255};

    Color sky;
    sky.r = LerpByte(bright.r, dark.r, skyDarkness);
    sky.g = LerpByte(bright.g, dark.g, skyDarkness);
    sky.b = LerpByte(bright.b, dark.b, skyDarkness);
    sky.a = 255;

    ClearBackground(sky);
}