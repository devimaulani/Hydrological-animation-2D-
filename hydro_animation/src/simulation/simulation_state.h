#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

#include "raylib.h"

typedef enum {
    PHASE_EVAP_TRANSP,      
    PHASE_CONDENSATION,     
    PHASE_ADVECTION,        
    PHASE_PRECIPITATION,    
    PHASE_INFILTR_COLLECT   
} CyclePhase;

typedef struct {
    float x;
    float y;
    float opacity;
    float darkProgress;  // 0.0 = putih, 1.0 = gelap penuh (transisi bertahap)
    bool isVisible;
} Cloud;

extern CyclePhase currentPhase;
extern float phaseTimer;

// Two managed Clouds
extern Cloud mainClouds[2];

// kontrol sistem
extern float evaporationRate;
extern float transpirationRate;
extern float sunIntensity;
extern float evaporationSpeedMult;
extern bool isWireframeMode;
extern bool applyWireframeColor;

// hasil (dipakai cloud)
extern float cloudDensity;

// posisi matahari (Celestial Arc)
extern float sunX;
extern float sunY;
extern float sunAngle;

// Tingkat gelap langit (0.0 = cerah, 1.0 = gelap) - transisi bertahap
extern float skyDarkness;

// Posisi X dimana awan berhenti (tengah daratan)
extern float cloudStopX;

void UpdateSimulationState(float dt);
const char* GetPhaseName(CyclePhase phase);

#endif