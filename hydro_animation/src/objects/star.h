#ifndef STAR_H
#define STAR_H

/*
 * star.h - Sistem Bintang Kerlap-kerlip untuk Langit Malam
 *
 * Bintang muncul saat skyDarkness melewati STAR_APPEAR_THRESHOLD,
 * dan memudar smooth saat langit kembali terang.
 */

/* Jumlah bintang */
#define STAR_COUNT 80

/*
 * Threshold skyDarkness sebelum bintang mulai muncul.
 * Diturunkan ke 0.15f agar bintang tampil pada fase Advection
 * (skyDarkness ~0.4) dan Precipitation (skyDarkness 0.7).
 */
#define STAR_APPEAR_THRESHOLD 0.15f

typedef struct {
    float x;             /* Posisi X pixel layar */
    float y;             /* Posisi Y pixel layar */
    float size;          /* Radius bintang (px) */
    float twinkleFreq;   /* Frekuensi kerlip (Hz) */
    float twinklePhase;  /* Fase awal (rad) */
    float twinkleAmp;    /* Amplitudo kerlip (0.2 - 0.65) */
    float baseAlpha;     /* Alpha dasar saat penuh terlihat */
    float currentAlpha;  /* Alpha saat ini (interpolasi smooth) */
    float targetAlpha;   /* Alpha tujuan berdasarkan skyDarkness */
} Star;

/* Inisialisasi bintang. screenH_sky = tinggi area langit dalam pixel */
void InitStars(int screenW, int screenH_sky);

/* Update twinkle & fade. Panggil tiap frame sebelum DrawStars */
void UpdateStars(float dt, float skyDarkness);

/* Gambar semua bintang */
void DrawStars(void);

#endif /* STAR_H */