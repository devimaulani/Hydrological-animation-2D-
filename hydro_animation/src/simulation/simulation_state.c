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

float evaporationRate   = 1.0f;
float transpirationRate = 1.0f;
float sunIntensity      = 1.0f;

float cloudDensity      = 0.0f;

// TWO MANAGED CLOUDS
// Cloud[0]: Sea Cloud, Cloud[1]: Transition Cloud
// darkProgress dimulai dari 0.0 (putih)
Cloud mainClouds[2] = {
    {300.0f, 220.0f, 0.0f, 0.0f, true},
    {50.0f, 260.0f, 0.0f, 0.0f, true}
};

// Posisi matahari (Celestial Arc)
float sunX = 0;
float sunY = 0;
float sunAngle = 3.14159f;

// Tingkat gelap langit (transisi bertahap)
float skyDarkness = 0.0f;

// Target skyDarkness untuk transisi smooth
static float skyDarknessTarget = 0.0f;

// Posisi X dimana awan berhenti (tengah daratan, sedikit ke kanan agar tidak terpotong)
float cloudStopX = -130.0f;

// Kecepatan transisi langit (semakin kecil = semakin lembut)
#define SKY_TRANSITION_SPEED 0.3f

// Kecepatan transisi gelap awan 
#define CLOUD_DARK_SPEED 0.15f

void UpdateSimulationState(float dt)
{
    phaseTimer += dt;

    // --- LOGIKA MATAHARI BERDASARKAN LINTASAN GAMBAR (Garis Merah) ---
    // radius horizontal diperlebar (650) dan radius vertikal ditinggikan (400) 
    // agar lintasan melengkung sempurna dari kiri gunung ke kanan laut.
    float rX = 650.0f;
    float rY = 420.0f;
    float baseY = -80.0f; // Menurunkan titik nol busur agar muncul dari balik gunung
    
    switch (currentPhase) {
        case PHASE_EVAP_TRANSP:
            // Terbit dari balik gunung di kiri (Sudut: ~140 derajat) menuju puncak (90 derajat)
            {
                float progress = phaseTimer / 8.0f;
                if (progress > 1.0f) progress = 1.0f;
                sunAngle = 2.45f - (progress * (2.45f - 1.57f)); // 140° ke 90°
            }
            break;
        case PHASE_CONDENSATION:
            // Tetap di puncak (90 derajat) saat awan menebal
            sunAngle = 1.57f;
            break;
        case PHASE_ADVECTION:
            // Mulai turun dari puncak (90 derajat) menuju sisi kanan (45 derajat) saat awan datang
            {
                float progress = phaseTimer / 8.0f;
                if (progress > 1.0f) progress = 1.0f;
                sunAngle = 1.57f - (progress * (1.57f - 0.78f)); // 90° ke 45°
            }
            break;
        case PHASE_PRECIPITATION:
            // Matahari turun tajam menuju laut (45° ke -30°) sambil memudar saat hujan
            {
                float progress = phaseTimer / 12.0f;
                if (progress > 1.0f) progress = 1.0f;
                sunAngle = 0.78f - (progress * (0.78f - (-0.52f))); 
            }
            break;
        case PHASE_INFILTR_COLLECT:
            // Tetap sembunyi di bawah laut (-30°) sampai siklus reset
            sunAngle = -0.52f;
            break;
    }

    sunX = rX * cosf(sunAngle);
    sunY = rY * sinf(sunAngle) + baseY; 


    switch (currentPhase) {
        case PHASE_EVAP_TRANSP:
            sunIntensity = 1.0f;
            skyDarknessTarget = 0.0f;  // Langit cerah
            // Awan bening -> Putih pekat saat uap naik (Laju diperlambat agar sinkron dengan durasi 15s)
            mainClouds[0].opacity += evaporationRate * dt * 0.06f;
            mainClouds[1].opacity += transpirationRate * dt * 0.06f;
            // Warna awan kembali ke putih secara bertahap
            mainClouds[0].darkProgress -= 0.3f * dt;
            mainClouds[1].darkProgress -= 0.3f * dt;
            if (mainClouds[0].opacity >= 0.8f && mainClouds[1].opacity >= 0.8f) {
                if (phaseTimer > 15.0f) {
                    currentPhase = PHASE_CONDENSATION;
                    phaseTimer = 0.0f;
                }
            }
            break;

        case PHASE_CONDENSATION:
            // Sempurnakan ketebalan awan
            mainClouds[0].opacity += 0.15f * dt;
            mainClouds[1].opacity += 0.15f * dt;
            if (mainClouds[0].opacity >= 1.0f && mainClouds[1].opacity >= 1.0f) {
                mainClouds[0].opacity = 1.0f;
                mainClouds[1].opacity = 1.0f;
                if (phaseTimer > 8.0f) {
                    currentPhase = PHASE_ADVECTION;
                    phaseTimer = 0.0f;
                    ResetAdvection();  // Reset angin saat siklus adveksi dimulai
                }
            }
            break;

        case PHASE_ADVECTION: {
            // Geser awan ke arah gunung (Kiri) sampai posisi berhenti (tengah daratan)
            float cloudSpeed = 70.0f;
            
            // Geser awan hanya jika belum sampai posisi berhenti
            if (mainClouds[0].x > cloudStopX) {
                mainClouds[0].x -= cloudSpeed * dt;
                mainClouds[1].x -= cloudSpeed * dt;
            }
            
            // Clamp posisi awan agar tidak lewat dari posisi berhenti
            if (mainClouds[0].x < cloudStopX) {
                mainClouds[0].x = cloudStopX;
            }
            if (mainClouds[1].x < cloudStopX - 200.0f) {
                mainClouds[1].x = cloudStopX - 200.0f;
            }

            // Mulai menggelapkan awan BERTAHAP saat bergeser
            float advectionProgress = phaseTimer / 8.0f;
            if (advectionProgress > 1.0f) advectionProgress = 1.0f;
            mainClouds[0].darkProgress += CLOUD_DARK_SPEED * dt;
            mainClouds[1].darkProgress += CLOUD_DARK_SPEED * dt;
            
            // Langit mulai sedikit gelap saat awan datang
            skyDarknessTarget = advectionProgress * 0.4f;

            if (phaseTimer > 8.0f) {
                currentPhase = PHASE_PRECIPITATION;
                phaseTimer = 0.0f;
            }
            break;
        }

        case PHASE_PRECIPITATION:
            sunIntensity = 0.4f;
            skyDarknessTarget = 0.7f;  // Langit gelap saat hujan
            // Awan tetap gelap sepenuhnya
            mainClouds[0].darkProgress += CLOUD_DARK_SPEED * dt;
            mainClouds[1].darkProgress += CLOUD_DARK_SPEED * dt;
            if (phaseTimer > 12.0f) {
                currentPhase = PHASE_INFILTR_COLLECT;
                phaseTimer = 0.0f;
            }
            break;

        case PHASE_INFILTR_COLLECT:
            sunIntensity = 0.7f;
            skyDarknessTarget = 0.2f;  // Langit mulai cerah kembali
            // Slide OUT ke arah kiri layar - Diperlambat agar durasi koleksi/infiltrasi lebih lama
            mainClouds[0].x -= 65.0f * dt;
            mainClouds[1].x -= 65.0f * dt;
            // Menipis kembali + awan kembali terang
            mainClouds[0].opacity -= 0.1f * dt;
            mainClouds[1].opacity -= 0.1f * dt;
            mainClouds[0].darkProgress -= 0.15f * dt;
            mainClouds[1].darkProgress -= 0.15f * dt;

            // Reset siklus jika sudah di luar layar kiri (-600)
            if (mainClouds[0].x < -600.0f && mainClouds[1].x < -600.0f) {
                currentPhase = PHASE_EVAP_TRANSP;
                phaseTimer = 0.0f;
                skyDarknessTarget = 0.0f;
                // Reset posisi ke kanan (Slide IN di siklus berikutnya)
                mainClouds[0].x = 300.0f;
                mainClouds[0].opacity = 0.0f;
                mainClouds[0].darkProgress = 0.0f;
                
                mainClouds[1].x = 50.0f;
                mainClouds[1].opacity = 0.0f;
                mainClouds[1].darkProgress = 0.0f;

                ResetAdvection(); 
                InitRain();
            }
            break;
    }

    // === TRANSISI BERTAHAP LANGIT (smooth lerp) ===
    if (skyDarkness < skyDarknessTarget) {
        skyDarkness += SKY_TRANSITION_SPEED * dt;
        if (skyDarkness > skyDarknessTarget) skyDarkness = skyDarknessTarget;
    } else if (skyDarkness > skyDarknessTarget) {
        skyDarkness -= SKY_TRANSITION_SPEED * dt;
        if (skyDarkness < skyDarknessTarget) skyDarkness = skyDarknessTarget;
    }
    // Clamp sky darkness
    if (skyDarkness < 0.0f) skyDarkness = 0.0f;
    if (skyDarkness > 1.0f) skyDarkness = 1.0f;

    // Clamp opacities & darkProgress
    for(int i=0; i<2; i++) {
        if (mainClouds[i].opacity > 1.0f) mainClouds[i].opacity = 1.0f;
        if (mainClouds[i].opacity < 0.0f) mainClouds[i].opacity = 0.0f;
        if (mainClouds[i].darkProgress > 1.0f) mainClouds[i].darkProgress = 1.0f;
        if (mainClouds[i].darkProgress < 0.0f) mainClouds[i].darkProgress = 0.0f;
    }
}

const char* GetPhaseName(CyclePhase phase) {
    switch (phase) {
        case PHASE_EVAP_TRANSP:     return "Evaporasi & Transpirasi (Pembentukan Awan)";
        case PHASE_CONDENSATION:    return "Kondensasi (Awan Menebal)";
        case PHASE_ADVECTION:       return "Adveksi (Awan Bergeser ke Daratan)";
        case PHASE_PRECIPITATION:   return "Presipitasi (Turun Hujan)";
        case PHASE_INFILTR_COLLECT: return "Infiltrasi & Koleksi Air (Awan Keluar)";
        default:                    return "";
    }
}