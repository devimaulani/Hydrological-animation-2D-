#include "simulation_state.h"

// ====== PARAMETER ======
float evaporationRate   = 1.0f;
float transpirationRate = 1.0f;
float sunIntensity      = 1.0f;   // 1 = siang penuh

// ====== STATE ======
float cloudDensity      = 0.0f;

// ====== UPDATE ======
void UpdateSimulationState(float dt)
{
    // total uap dipengaruhi matahari
    float totalVapor = (evaporationRate + transpirationRate) * sunIntensity;

    // akumulasi ke awan
    cloudDensity += totalVapor * dt * 0.15f;

    // clamp
    if (cloudDensity > 1.0f) cloudDensity = 1.0f;
    if (cloudDensity < 0.0f) cloudDensity = 0.0f;
}