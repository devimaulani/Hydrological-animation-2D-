#include "info_screens.h"
#include "../ui/button.h"
#include "../../screen_type.h"
#include "../../coords.h"
#include "../objects/mountain.h"
#include "../objects/water.h"
#include "../../src/wrapper/draw_rectangle.h"
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
    
    // Add semi-transparent overlay to clear text areas (Using Wrapper)
    Wrapper_DrawRectangleFilled(300, 100, SCREEN_W - 400, SCREEN_H - 350, (Color){ 255, 255, 255, 200 });
    Wrapper_DrawRectangleOutline(300, 100, SCREEN_W - 400, SCREEN_H - 350, 2, DARKGRAY);
}

void DrawRainInfoScreen() {
    // 1. Background khusus luas
    ClearBackground((Color){ 200, 230, 255, 255 }); 
    DrawMountain();
    DrawWater(-106);
    
    // Panel Informasi Transparan (Tinggi dikurangi agar tidak menabrak tombol)
    Wrapper_DrawRectangleFilled(70, 70, SCREEN_W - 140, SCREEN_H - 220, (Color){ 255, 255, 255, 245 });
    Wrapper_DrawRectangleOutline(70, 70, SCREEN_W - 140, SCREEN_H - 220, 4, DARKGRAY);

    // JUDUL UTAMA (Bold)
    const char* title = "TAHAPAN LENGKAP SIKLUS HIDROLOGI";
    DrawText(title, 101, 91, 28, BLACK); 
    DrawText(title, 100, 90, 28, DARKBLUE);
    
    int startY = 160;
    int spacing = 52;
    int col1X = 100;
    int col2X = 660;

    // STEP-BY-STEP (Bold Headers)
    // KOLOM 1
    DrawText("1. EVAPORASI", col1X+1, startY, 18, MAROON);
    DrawText("1. EVAPORASI", col1X, startY, 18, MAROON);
    DrawText("Penguapan air laut/danau akibat panas matahari.", col1X, startY + 22, 17, DARKGRAY);

    DrawText("2. TRANSPIRASI", col1X+1, startY + spacing, 18, MAROON);
    DrawText("2. TRANSPIRASI", col1X, startY + spacing, 18, MAROON);
    DrawText("Penguapan air dari jaringan mahluk hidup (tumbuhan).", col1X, startY + spacing + 22, 17, DARKGRAY);

    DrawText("3. KONDENSASI", col1X+1, startY + spacing*2, 18, MAROON);
    DrawText("3. KONDENSASI", col1X, startY + spacing*2, 18, MAROON);
    DrawText("Uap mendingin & membentuk titik air (awan tebal).", col1X, startY + spacing*2 + 22, 17, DARKGRAY);

    DrawText("4. ADVEKSI", col1X+1, startY + spacing*3, 18, MAROON);
    DrawText("4. ADVEKSI", col1X, startY + spacing*3, 18, MAROON);
    DrawText("Pergerakan awan ditiup angin dari laut ke darat.", col1X, startY + spacing*3 + 22, 17, DARKGRAY);

    // KOLOM 2
    DrawText("5. PRESIPITASI", col2X+1, startY, 18, MAROON);
    DrawText("5. PRESIPITASI", col2X, startY, 18, MAROON);
    DrawText("Jatuhnya butiran air dari awan (hujan) ke bumi.", col2X, startY + 22, 17, DARKGRAY);

    DrawText("6. INFILTRASI", col2X+1, startY + spacing, 18, MAROON);
    DrawText("6. INFILTRASI", col2X, startY + spacing, 18, MAROON);
    DrawText("Penyerapan air hujan ke dalam lapisan pori-pori tanah.", col2X, startY + spacing + 22, 17, DARKGRAY);

    DrawText("7. RUN-OFF / KOLEKSI", col2X+1, startY + spacing*2, 18, MAROON);
    DrawText("7. RUN-OFF / KOLEKSI", col2X, startY + spacing*2, 18, MAROON);
    DrawText("Mengalirnya air di permukaan kembali ke laut/sungai.", col2X, startY + spacing*2 + 22, 17, DARKGRAY);

    DrawText("(?) Arahkan kursor ke objek saat simulasi untuk detail.", col2X, startY + spacing*4, 17, DARKBLUE);
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void DrawShapeInfoScreen() {
    // 1. Background khusus yang lebih luas
    ClearBackground((Color){ 200, 230, 255, 255 }); 
    DrawMountain();
    DrawWater(-106);
    
    // Panel Informasi Transparan (Lebih Luas)
    Wrapper_DrawRectangleFilled(70, 70, SCREEN_W - 140, SCREEN_H - 180, (Color){ 255, 255, 255, 240 });
    Wrapper_DrawRectangleOutline(70, 70, SCREEN_W - 140, SCREEN_H - 180, 4, DARKGRAY);

    // JUDUL UTAMA (Dipertebal)
    const char* mainTitle = "TEKNIS GRAFIKA KOMPUTER (OBJECT ALGORITHMS)";
    DrawText(mainTitle, 101, 91, 28, BLACK); // Bayangan tipis
    DrawText(mainTitle, 100, 90, 28, DARKBLUE);
    
    // KOLOM KIRI & KANAN (Pengaturan Jarak Baru)
    int leftX = 90;
    int rightX = 660; // Geser lebih ke kanan agar tidak bertabrakan
    int headY = 150;
    int startY = 190;
    int spacing = 50;

    // Headings (Bold Effect)
    DrawText("ATMOSFER & MEDAN KIRI", leftX + 1, headY, 20, MAROON);
    DrawText("ATMOSFER & MEDAN KIRI", leftX, headY, 20, MAROON);
    
    DrawText("VEGETASI & AIR", rightX + 1, headY, 20, MAROON);
    DrawText("VEGETASI & AIR", rightX, headY, 20, MAROON);

    // KONTEN KIRI (Simulated Bold)
    DrawText("- MATAHARI: Mid-Point Circle & Halo Rings dinamis.", leftX, startY, 17, DARKGRAY);
    DrawText("- LANGIT: Scanline Gradient (Rectangle Fill).", leftX, startY + spacing, 17, DARKGRAY);
    DrawText("- AWAN: 6 kluster elips (Mid-Point Ellipse).", leftX, startY + spacing*2, 17, DARKGRAY);
    DrawText("- BURUNG: Parametrik & Sayap Sinusoidal.", leftX, startY + spacing*3, 17, DARKGRAY);
    DrawText("- GUNUNG: Bresenham Line (Thick Line).", leftX, startY + spacing*4, 17, DARKGRAY);
    DrawText("- TANAH: Multi-layer Polygon & Ground Wave.", leftX, startY + spacing*5, 17, DARKGRAY);

    // KONTEN KANAN (Simulated Bold)
    DrawText("- SUNGAI: Ombak dinamis fungsi sinf().", rightX, startY, 17, DARKGRAY);
    DrawText("- DANAU: Scanline & Osilasi Boundary.", rightX, startY + spacing, 17, DARKGRAY);
    DrawText("- POHON: Trunk (Thick) & Leaves (Ellipse).", rightX, startY + spacing*2, 17, DARKGRAY);
    DrawText("- BUNGA: Center (Circle) & Petal (Ellipse).", rightX, startY + spacing*3, 17, DARKGRAY);
    DrawText("- RUMPUT: Bresenham & Swaying Dinamis.", rightX, startY + spacing*4, 17, DARKGRAY);
    DrawText("- BATU: Ellipse statis & Shading Berlapis.", rightX, startY + spacing*5, 17, DARKGRAY);
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void DrawAboutScreen() {
    // Background Konsisten
    ClearBackground((Color){ 200, 230, 255, 255 }); 
    DrawMountain();
    DrawWater(-106);

    // Panel Informasi Tengah
    Wrapper_DrawRectangleFilled(150, 100, SCREEN_W - 300, SCREEN_H - 250, (Color){ 255, 255, 255, 245 });
    Wrapper_DrawRectangleOutline(150, 100, SCREEN_W - 300, SCREEN_H - 250, 4, DARKGRAY);
    
    // JUDUL (Bold & Center)
    const char* title = "TENTANG PEMBUAT";
    int twTitle = MeasureText(title, 30);
    DrawText(title, SCREEN_W/2 - twTitle/2 + 1, 131, 30, BLACK);
    DrawText(title, SCREEN_W/2 - twTitle/2, 130, 30, DARKBLUE);
    
    int startY = 200;
    int spacing = 35;
    const char* lines[] = {
        "Nama: Devi Maulani",
        "NIM: 241524007",
        "Mata Kuliah: Grafik Komputer",
        "Tugas Besar: Animasi 2D Hidrologi",
        "",
        "Teknologi:",
        "- Bahasa Pemrograman C",
        "- Raylib Graphic Library",
        "- Custom Primitives Library"
    };

    for(int i = 0; i < 9; i++) {
        if (TextLength(lines[i]) == 0) continue;
        int tw = MeasureText(lines[i], 20);
        // Simulated Bold
        DrawText(lines[i], SCREEN_W/2 - tw/2 + 1, startY + i*spacing, 20, BLACK);
        DrawText(lines[i], SCREEN_W/2 - tw/2, startY + i*spacing, 20, DARKGRAY);
    }
    
    if (DrawButton(&btnBack)) currentScreen = MENU_SCREEN;
}

void UnloadInfoScreens() {
    // Nothing special to unload
}
