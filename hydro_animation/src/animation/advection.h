#ifndef ADVECTION_H
#define ADVECTION_H

void InitAdvection();
void UpdateAdvection(float dt);
void DrawAdvection();

// 🔥 status angin sudah sampai darat
int IsWindReachedLand();

#endif