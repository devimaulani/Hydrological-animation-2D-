#include "simulation_state.h"
#include "../animation/advection.h"
#include "../animation/precipitation.h"
#include "../animation/evaporation.h"
#include "../animation/transpiration.h"
#include "coords.h"
#include <stdio.h>
#include <math.h>

CyclePhase currentPhase = PHASE_EVAP_TRANSP;
float phaseTimer = 0.0f;

float evaporationRate   = 0.0f; 
float transpirationRate = 0.0f;
float sunIntensity      = 1.0f;
float evaporationSpeedMult = 1.0f;
bool isWireframeMode    = false;
bool applyWireframeColor = false;
float skyDarkness       = 0.0f;

float cloudDensity      = 0.0f;
float cloudStopX        = -160.0f; // Titik henti awan pas di tengah daratan

// Cloud[0]: Sea Cloud, Cloud[1]: Transition Cloud
Cloud mainClouds[2] = {
    {300.0f, 220.0f, 0.0f, 0.0f, true},
    {50.0f, 260.0f, 0.0f, 0.0f, true}
};

float sunX = 0;
float sunY = 0;
float sunAngle = 3.14159f;

void UpdateSimulationState(float dt)
{
    phaseTimer += dt;

    // 1. Matahari (Celestial Arc)
    float speed = 0.05f; 
    sunAngle -= (speed * dt);
    float rX = 400.0f;
    float rY = 250.0f;
    sunX = rX * cosf(sunAngle);
    sunY = rY * sinf(sunAngle) + 100.0f; 
    if (sunAngle < 0) sunAngle = 3.14159f;

    // 2. Transisi Gelap Langit (Smooth Lerp)
    float targetDarkness = 0.0f;
    if (currentPhase == PHASE_PRECIPITATION) targetDarkness = 1.0f;
    else if (currentPhase == PHASE_ADVECTION) targetDarkness = (phaseTimer / 8.0f) * 0.7f; 

    if (skyDarkness < targetDarkness) skyDarkness += 0.4f * dt;
    else if (skyDarkness > targetDarkness) skyDarkness -= 0.4f * dt;

    switch (currentPhase) {
        case PHASE_EVAP_TRANSP:
            sunIntensity = 1.0f;
            evaporationRate += 0.3f * dt;
            transpirationRate += 0.3f * dt;
            if (evaporationRate > 1.0f) evaporationRate = 1.0f;
            if (transpirationRate > 1.0f) transpirationRate = 1.0f;

            mainClouds[0].opacity += 0.15f * dt;
            mainClouds[1].opacity += 0.15f * dt;
            
            if (mainClouds[0].opacity >= 1.0f && phaseTimer > 8.0f) {
                currentPhase = PHASE_CONDENSATION;
                phaseTimer = 0.0f;
            }
            break;

        case PHASE_CONDENSATION:
            if (phaseTimer > 5.0f) {
                currentPhase = PHASE_ADVECTION;
                phaseTimer = 0.0f;
            }
            break;

        case PHASE_ADVECTION:
            // Geser awan ke arah gunung tapi berhenti di TENGAH LAND
            if (mainClouds[0].x > cloudStopX) mainClouds[0].x -= 75.0f * dt;
            // Awan kedua berhenti sedikit lebih ke kiri dari awan pertama
            if (mainClouds[1].x > cloudStopX - 100.0f) mainClouds[1].x -= 75.0f * dt;

            // Transisi awan putih -> gelap secara bertahap
            mainClouds[0].darkProgress += 0.2f * dt;
            mainClouds[1].darkProgress += 0.2f * dt;

            if (phaseTimer > 10.0f) {
                currentPhase = PHASE_PRECIPITATION;
                phaseTimer = 0.0f;
            }
            break;

        case PHASE_PRECIPITATION:
            sunIntensity = 0.4f;
            if (phaseTimer > 14.0f) {
                currentPhase = PHASE_INFILTR_COLLECT;
                phaseTimer = 0.0f;
            }
            break;

        case PHASE_INFILTR_COLLECT:
            sunIntensity = 0.7f;
            // Awan bergeser keluar layar ke KIRI
            mainClouds[0].x -= 120.0f * dt;
            mainClouds[1].x -= 120.0f * dt;

            evaporationRate -= 0.5f * dt;
            transpirationRate -= 0.5f * dt;
            if (evaporationRate < 0) evaporationRate = 0;
            if (transpirationRate < 0) transpirationRate = 0;

            mainClouds[0].opacity -= 0.2f * dt;
            mainClouds[1].opacity -= 0.2f * dt;

            if (mainClouds[0].x < -600.0f) {
                currentPhase = PHASE_EVAP_TRANSP;
                phaseTimer = 0.0f;
                // Reset posisi ke kanan
                mainClouds[0].x = 400.0f;
                mainClouds[0].darkProgress = 0.0f;
                mainClouds[1].x = 150.0f;
                mainClouds[1].darkProgress = 0.0f;
                ResetAdvection(); 
                InitRain();
            }
            break;
    }

    // Clamping
    if (skyDarkness > 1.0f) skyDarkness = 1.0f;
    if (skyDarkness < 0.0f) skyDarkness = 0.0f;
    for(int i=0; i<2; i++) {
        if (mainClouds[i].darkProgress > 1.0f) mainClouds[i].darkProgress = 1.0f;
        if (mainClouds[i].opacity > 1.0f) mainClouds[i].opacity = 1.0f;
        if (mainClouds[i].opacity < 0.0f) mainClouds[i].opacity = 0.0f;
    }
}

const char* GetPhaseName(CyclePhase phase) {
    switch (phase) {
        case PHASE_EVAP_TRANSP:     return "Awal Penguapan (Vapor Fading)";
        case PHASE_CONDENSATION:    return "Kondensasi (Pembentukan Awan)";
        case PHASE_ADVECTION:       return "Adveksi (Awan Menuju Tengah Darat)";
        case PHASE_PRECIPITATION:   return "Presipitasi (Hujan di Dua Ketinggian)";
        case PHASE_INFILTR_COLLECT: return "Infiltrasi (Awan Geser Keluar)";
        default:                    return "";
    }
}