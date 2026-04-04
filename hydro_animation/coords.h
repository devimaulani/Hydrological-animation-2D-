#include "screen_type.h"

#ifndef COORDS_H
#define COORDS_H

#define HALF_W (SCREEN_W / 2)
#define HALF_H (SCREEN_H / 2)

#define LEFT   (-HALF_W)
#define RIGHT  (HALF_W)
#define TOP    (HALF_H)
#define BOTTOM (-HALF_H)

// koordinat global
extern int G_OriginX;
extern int G_OriginY;
extern float G_TickStep;

// TAMBAHAN (penting untuk scaling)
extern int BASE_WIDTH;
extern int BASE_HEIGHT;

// fungsi konversi (integer)
int CS_X(int cart_x);
int CS_Y(int cart_y);

// fungsi konversi (float untuk presisi pergerakan)
float CS_Xf(float cart_x);
float CS_Yf(float cart_y);

#endif