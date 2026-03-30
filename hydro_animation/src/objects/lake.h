#ifndef LAKE_H
#define LAKE_H

#include "coords.h"

// =========================================================
// LAKE MODULE - Danau Organik + Aliran Sungai
// =========================================================

#define LAKE_CENTER_X   (LEFT + 370)    // (sebelumnya: LEFT + 340)
#define LAKE_CENTER_Y   (BOTTOM + 260)  // (sebelumnya: BOTTOM + 185)
#define LAKE_RADIUS_X   110             // (sebelumnya: 88)
#define LAKE_RADIUS_Y   50              // (sebelumnya: 38)

#define LAKE_BOTTOM_Y   (LAKE_CENTER_Y - LAKE_RADIUS_Y)

void InitLake();
void DrawLake();

#endif