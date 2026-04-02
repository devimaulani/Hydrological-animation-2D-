#include "info_screens.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "../../coords.h"
#include "../objects/mountain.h"
#include "../objects/water.h"
#include "raylib.h"

static Button btnBack;

void InitInfoScreens() {
    btnBack = CreateButton(50, SCREEN_H - 100, 200, 50, "<< Kembali");
}

static void DrawInfoBackground() {
    ClearBackground((Color){ 200, 230, 255, 255 }); // Pale Blue Sky
    
    // Reuse existing graphics for decoration
    DrawMountain();
    DrawWater(-106);
    
    // Add semi-transparent overlay to clear text areas
    DrawRectangle(300, 100, SCREEN_W - 400, SCREEN_H - 350, (Color){ 255, 255, 255, 200 });
    DrawRectangleLines(300, 100, SCREEN_W - 400, SCREEN_H - 350, DARKGRAY);
}

void DrawRainInfoScreen() {
    DrawInfoBackground();
    
    DrawText("PENJELASAN SIKLUS HIDROLOGI", 320, 120, 30, DARKBLUE);
    
    const char *info = 
        "1. Evaporasi: Air laut menguap karena panas matahari.\n\n"
        "2. Transpirasi: Penguapan air dari jaringan tumbuhan.\n\n"
        "3. Adveksi: Awan bergerak ditiup angin ke daratan.\n\n"
        "4. Presipitasi: Proses jatuhnya hujan ke permukaan bumi.\n\n"
        "5. Infiltrasi: Penyerapan air hujan ke dalam lapisan tanah.";
        
    DrawText(info, 320, 180, 20, BLACK);
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void DrawShapeInfoScreen() {
    DrawInfoBackground();
    
    DrawText("TEKNIS GRAFIKA KOMPUTER", 320, 120, 30, DARKBLUE);
    
    const char *info = 
        "- Gunung: Dibentuk menggunakan algoritma poligon dinamis\n  dengan ketebalan garis (Wrapper_DrawLineThick).\n\n"
        "- Laut: Menggunakan fungsi sinf() dan cosf() untuk\n  simulasi ombak yang bergerak dinamis.\n\n"
        "- Infiltrasi: Partikel air tanah menggunakan sistem\n  Midpoint Circle Algorithm (MidcircleFilled).\n\n"
        "- Awan & Pohon: Dibentuk dari gabungan algoritma\n  Midpoint Ellipse untuk tekstur yang natural.";
        
    DrawText(info, 320, 180, 20, BLACK);
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void DrawAboutScreen() {
    DrawInfoBackground();
    
    DrawText("TENTANG PEMBUAT", 320, 120, 30, DARKBLUE);
    
    const char *info = 
        "Nama: Devi Maulani\n"
        "Mata Kuliah: Grafika Komputer\n"
        "Tugas Besar (TUBES) - Animasi 2D Hidrologi\n\n"
        "Teknologi:\n"
        "- Bahasa C\n"
        "- Raylib Framework\n"
        "- Custom Primitives Library";
        
    DrawText(info, 320, 180, 20, BLACK);
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void UnloadInfoScreens() {
    // Nothing special to unload
}
