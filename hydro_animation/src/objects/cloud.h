#ifndef CLOUD_H
#define CLOUD_H

#include "raylib.h"

// Awan dinamis yang mendukung modularitas koordinat, opacity, dan warna mendung
void DrawCloud(float x, float y, float opacity);
// darkProgress: 0.0 = putih, 1.0 = gelap penuh (transisi bertahap)
void DrawCloudDynamic(float x, float y, float opacity, float darkProgress);

#endif