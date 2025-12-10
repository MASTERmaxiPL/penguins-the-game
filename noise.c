#include "noise.h"
#include <stdint.h>
#include <math.h>

/**
 * @file noise.c
 * @brief Value-noise and FBM implementation for procedural board generation.
 *
 * The implementation provides deterministic per-grid-point hashing, value noise
 * interpolation (with quintic fade), and an FBM function that composes octaves.
 */

/* -- internal helpers (hash, fade, lerp, value noise) -- */

static inline uint32_t Hash_Coords(int x, int y, unsigned int seed) {
    uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u + seed * 1442695040888963407u;
    h = (h ^ (h >> 13)) * 1274126177u;
    return h;
}

static inline double Fade(double t) {
    /* 6t^5 - 15t^4 + 10t^3 */
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline double Lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

static double Value_Noise(double x, double y, unsigned int seed) {
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    double xf = x - (double)x0;
    double yf = y - (double)y0;

    double v00 = (double)(Hash_Coords(x0, y0, seed) & 0xFFFFu) / (double)0xFFFFu;
    double v10 = (double)(Hash_Coords(x0+1, y0, seed) & 0xFFFFu) / (double)0xFFFFu;
    double v01 = (double)(Hash_Coords(x0, y0+1, seed) & 0xFFFFu) / (double)0xFFFFu;
    double v11 = (double)(Hash_Coords(x0+1, y0+1, seed) & 0xFFFFu) / (double)0xFFFFu;

    double u = Fade(xf);
    double v = Fade(yf);

    double ix0 = Lerp(v00, v10, u);
    double ix1 = Lerp(v01, v11, u);

    return Lerp(ix0, ix1, v);
}

double Fbm(double noiseX, double noiseY, int octaveCount, double lacunarity, double gain, unsigned int seed) {
    double amplitude = 1.0;
    double frequency = 1.0;
    double sum = 0.0;
    double ampSum = 0.0;

    for (int i = 0; i < octaveCount; ++i) {
        double nx = noiseX * frequency;
        double ny = noiseY * frequency;
        double val = Value_Noise(nx, ny, seed + (unsigned int)i * 57u);
        sum += val * amplitude;
        ampSum += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    if (ampSum != 0.0)
        return sum / ampSum;
    return 0.0;
}
