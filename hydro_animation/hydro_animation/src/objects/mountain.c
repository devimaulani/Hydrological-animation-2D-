#include "mountain.h"
#include "../wrapper/draw_line.h"
#include "coords.h"
#include <math.h>

// Fungsi untuk menggambar satu gunung dengan bentuk kurva lonceng / cosinus
// Parameter:
//   cx    : Titik tengah (puncak) gunung
//   w     : Lebar setengah gunung (radius horizontal)
//   h     : Tinggi maksimal gunung
//   color : Warna gunung
//   baseY : Ketinggian dasar (tanah)
static void DrawSingleMountain(int cx, int w, int h, Color color, int baseY) {
    for (int x = cx - w; x <= cx + w; x++) {
        // Keamanan: pastikan gunung tidak pernah melewati batas awal tebing laut (di x = 100)
        // Sehingga gunung DIJAMIN hanya berada di area daratan rumput
        if (x > 90) {
            continue; 
        }

        // Hitung persentase jarak x dari titik tengah gunung (-1.0 sampai 1.0)
        float t = (float)(x - cx) / (float)w;
        
        // Membentuk profil gunung (menggunakan fungsi cosine dipangkatkan)
        // Pangkat 2.2 akan membuat gunung lebih terbentuk segi-tiga (tidak terlalu tumpul) tapi dasarnya tetap melandai natural
        float heightVal = h * powf(cosf(t * 3.14159f / 2.0f), 2.2f);
        int peakY = baseY + (int)heightVal;

        // Gambar pilar vertikal dari dasar tanah hingga ke permukaan gunung
        if (peakY > baseY) {
            // Ketebalan 2 untuk memastikan tidak ada celah kosong antar garis
            Wrapper_DrawLineThick(x, baseY, x, peakY, 2, color);
        }
    }
}

void DrawMountain() {
    // =========================
    // PARAMETER DASAR
    // =========================
    // Diselaraskan dengan awal daratan di land.c agar tidak ada celah udara
    int landTop = (int)(-HALF_H / 4.2f) - 5; 

    // =========================
    // MENGGAMBAR PEGUNUNGAN (DARI BELAKANG KE DEPAN)
    // Terdapat 5 puncak gunung yang digambar agar membentuk sebuah deretan pegunungan natural
    // =========================

    // LAPIS 1: Lapisan Paling Belakang (Paling redup warnanya karena jauh)
    //(semakin besar minus semakin ke kiri)
    DrawSingleMountain(-200, 160, 140, (Color){100, 150, 110, 255}, landTop);

    // LAPIS 2: Lapisan Tengah (Menjembatani antara gunung kiri dan kanan)
    DrawSingleMountain(-390, 170, 200, (Color){105, 160, 115, 255}, landTop);
    DrawSingleMountain(-100, 190, 180, (Color){105, 160, 115, 255}, landTop);

    // LAPIS 3: Lapisan Paling Depan (Warna paling tegas, puncak paling menonjol)
    DrawSingleMountain(-530, 210, 260, (Color){115, 175, 125, 255}, landTop);
    DrawSingleMountain(-290, 160, 180, (Color){115, 175, 125, 255}, landTop);
}