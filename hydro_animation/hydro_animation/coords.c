#include "coords.h"

// posisi origin (tengah layar)
int G_OriginX = 0;
int G_OriginY = 0;

// scaling
float G_TickStep = 1.0f;

int BASE_WIDTH = 1000;
int BASE_HEIGHT = 700;

// konversi koordinat
int CS_X(int cart_x) {
    return G_OriginX + (int)(cart_x * G_TickStep);
}

int CS_Y(int cart_y) {
    return G_OriginY - (int)(cart_y * G_TickStep);
}