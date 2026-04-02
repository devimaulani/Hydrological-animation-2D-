#include "simulation_screen.h"
#include "../objects/sky.h"
#include "../objects/sun.h"
#include "../objects/mountain.h"
#include "../objects/water.h"
#include "../objects/tree.h"
#include "../objects/land.h"
#include "../objects/cloud.h"
#include "../objects/lake.h"
#include "../objects/rock.h"
#include "../objects/grass.h"
#include "../objects/flower.h"
#include "../animation/evaporation.h"
#include "../animation/transpiration.h"
#include "../simulation/simulation_state.h"
#include "../animation/advection.h"
#include "../animation/precipitation.h"
#include "../animation/infiltration.h"
#include "../objects/birds.h"
#include "../controls/slider.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "coords.h"
#include <stdlib.h>

static Slider evapSlider;
static Button btnBackSim;

void InitSimulation() {
    InitEvaporation();
    InitTranspiration();
    InitAdvection();
    InitRain();
    InitLake();           // ← sebelum InitInfiltration
    InitInfiltration();
    InitBirds();          // Inisiasi awal posisi burung
    
    // Inisiasi Slider evaporasi (Nilai antara 0.2 hingga 2.5 dengan awalan 1.0)
    // Posisi: X Kiri, Y Atas
    InitSlider(&evapSlider, LEFT + 40, TOP - 40, 150, 0.2f, 2.5f, 1.0f);
    btnBackSim = CreateButton(20, 20, 150, 40, "Menu Utama");
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
    UpdateBirds(dt);      // Update gerak kepak sayap dan lintasan burung

    // Update Slider dan timpa var simulasi
    UpdateSlider(&evapSlider);
    evaporationRate = evapSlider.value;


    // draw
    DrawEvaporation(); //evaporasi dari laut
    DrawTranspiration(); //transpirasi dari pohon
    DrawAdvection(); //adveksi angin
    DrawRain(); //precipitation (hujan) 
    DrawInfiltration(); //infiltrasi ke tanah

    // Dekorasi alam (Batu, Rumput, Bunga)
    int landTop = (int)(-HALF_H / 4.2f); // landtop patokan dasar land 
    DrawRock(LEFT + 380, landTop + 5,  35, 18);
    DrawRock(LEFT + 410, landTop + 2,  20, 10);
    DrawRock(LEFT + 570, landTop + 5,  35, 18);
    DrawRock(LEFT + 670, landTop - 2,  25, 12);

    //DrawGrassTuft(LEFT + 250, landTop - 50);
    //DrawGrassTuft(LEFT + 290, landTop - 65);
    //DrawFlower(LEFT + 330,    landTop - 85);
    //DrawGrassTuft(LEFT + 460, landTop - 30);
    //DrawGrassTuft(LEFT + 490, landTop - 50);
    //DrawFlower(LEFT + 550,    landTop - 70);
    //DrawGrassTuft(LEFT + 750, landTop - 40);
    //DrawFlower(LEFT + 700,    landTop - 60);

    // posisi pohon 
    // Menggunakan landTop (garis dasar padang rumput) sebagai patokan elevasi Y
    DrawTree(LEFT + 240, landTop - 45);
    DrawTree(LEFT + 670, landTop - 55);
    DrawTree(LEFT + 280, landTop - 145);
    DrawTree(LEFT + 540, landTop - 130);
    DrawTree(LEFT + 740, landTop - 45);
    DrawTree(LEFT + 720, landTop - 120);

    //awan
    DrawCloud(RIGHT - 400, TOP - 250);
    DrawCloud(RIGHT - 300, TOP - 320);
    DrawCloud(RIGHT - 700, TOP - 320);

    DrawCloudDynamic(LEFT + 400, TOP - 250);
    DrawCloudDynamic(LEFT + 270, TOP - 200);

    // Burung digambar paling depan (atas awan dan pohon)
    DrawBirds();

    // Gambar UI paling atas
    DrawSlider(&evapSlider, "Evaporasi & Hujan");
    
    // Tombol kembali ke menu
    if (DrawButton(&btnBackSim)) currentScreen = MENU_SCREEN;
}