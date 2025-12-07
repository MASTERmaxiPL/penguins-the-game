#include "noise.h"

#include <stdint.h>
#include <math.h>

static unsigned int Hash_Coords(int x, int y, unsigned int seed);
static double Value_Noise(double x, double y, unsigned int seed);
static double Lerp(double a, double b, double t);
static double Fade(double t);

double Fbm(
    const double noiseX,
    const double noiseY,
    const int octaveCount,
    const double lacunarity,
    const double gain,
    const unsigned int seed
    ) {
    double amplitude = 1.0;
    double frequency = 1.0;
    double weightedSum = 0.0;
    double amplitudeSum = 0.0;

    for (int i = 0; i < octaveCount; i++) {
        weightedSum += amplitude * Value_Noise(noiseX * frequency, noiseY * frequency, seed + (unsigned int)i * 57u);
        amplitudeSum += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    return weightedSum / amplitudeSum;
}

static unsigned int Hash_Coords(const int x, const int y, const unsigned int seed) {
    const uint32_t kMulA = 374761393u;
    const uint32_t kMulB = 668265263u;
    const uint32_t kMulC = 1274126177u;

    uint32_t hashValue = (uint32_t)x * kMulA + (uint32_t)y * kMulB + (uint32_t)seed * kMulC;
    hashValue = (hashValue ^ (hashValue >> 13)) * kMulC;
    return hashValue;
}

static double Value_Noise(const double x, const double y, const unsigned int seed) {
    const int gridX = (int)floor(x);
    const int gridY = (int)floor(y);
    const int gridXNext = gridX + 1;
    const int gridYNext = gridY + 1;

    const double fracX = x - (double)gridX;
    const double fracY = y - (double)gridY;

    const uint32_t kValueMask = 0xFFFFu;
    const double valueNormalizer = kValueMask;

    const double value_top_left = (double)(Hash_Coords(gridX, gridY, seed) & kValueMask) / valueNormalizer;
    const double value_top_right = (double)(Hash_Coords(gridXNext, gridY, seed) & kValueMask) / valueNormalizer;
    const double value_bot_left = (double)(Hash_Coords(gridX, gridYNext, seed) & kValueMask) / valueNormalizer;
    const double value_bot_right = (double)(Hash_Coords(gridXNext, gridYNext, seed) & kValueMask) / valueNormalizer;

    const double weight_x = Fade(fracX);
    const double weight_y = Fade(fracY);

    const double interp_top = Lerp(value_top_left, value_top_right, weight_x);
    const double interp_bot = Lerp(value_bot_left, value_bot_right, weight_x);

    return Lerp(interp_top, interp_bot, weight_y);
}

static double Lerp(const double a, const double b, const double t) {
    return a + t * (b - a);
}

static double Fade(const double t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
