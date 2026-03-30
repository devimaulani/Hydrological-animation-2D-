#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// kontrol sistem
extern float evaporationRate;     // 0..2 (default 1)
extern float transpirationRate;   // 0..2 (default 1)
extern float sunIntensity;        // 0..1

// hasil (dipakai cloud)
extern float cloudDensity;        // 0..1

void UpdateSimulationState(float dt);

#endif