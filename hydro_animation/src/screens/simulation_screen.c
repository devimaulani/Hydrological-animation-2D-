#include "simulation_screen.h"
#include "../objects/sky.h"
#include "../objects/sun.h"
#include "../objects/mountain.h"
#include "../objects/water.h"
#include "../objects/tree.h"
#include "../objects/land.h"
#include "../objects/cloud.h"
#include "../objects/lake.h"
#include "../animation/evaporation.h"
#include "../animation/transpiration.h"
#include "../simulation/simulation_state.h"
#include "../animation/advection.h"
#include "../animation/precipitation.h"
#include "../animation/infiltration.h"
#include "coords.h"
#include <stdlib.h>


void InitSimulation() {
    InitEvaporation();
    InitTranspiration();
    InitAdvection();
    InitRain();
    InitLake();           // ← sebelum InitInfiltration
    InitInfiltration();
}


void DrawSimulation() {
 

    DrawSky(1);

    // matahari kanan
    DrawSun(RIGHT - 265, TOP - 200, 40);

    // gunung 
    DrawMountain();
    // land
    DrawLand();
    //danau 
    DrawLake();
    // water
    DrawWater(-106);

    // update & draw animasi
    float dt = GetFrameTime();
    // update state
    UpdateSimulationState(dt);
    // update animasi
    UpdateEvaporation(dt);
    UpdateTranspiration(dt);
    UpdateAdvection(dt);
    UpdateRain(dt);
    UpdateInfiltration(dt);

    // draw
    DrawEvaporation(); //evaporasi dari laut
    DrawTranspiration(); //transpirasi dari pohon
    DrawAdvection(); //adveksi angin
    DrawRain(); //precipitation (hujan) 
    DrawInfiltration(); //infiltrasi ke tanah

    // posisi pohon 
    DrawTree(LEFT + 240, -HALF_H / 3);
    DrawTree(LEFT + 600, -HALF_H / 2.8);
    DrawTree(LEFT + 250, -HALF_H / 1.5);
    DrawTree(LEFT + 640, -HALF_H / 2);
    DrawTree(LEFT + 670, -HALF_H / 3);

    //awan
    DrawCloud(RIGHT - 400, TOP - 250);
    DrawCloud(RIGHT - 300, TOP - 320);
    DrawCloud(RIGHT - 700, TOP - 320);

    DrawCloudDynamic(LEFT + 400, TOP - 250);
    DrawCloudDynamic(LEFT + 270, TOP - 200);
}