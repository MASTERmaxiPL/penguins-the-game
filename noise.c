/**
 * @file noise.c
 * @brief Implements FBM (Fractal Brownian Motion) and value noise functions
 *        used for procedural board generation.
 *
 * This module provides:
 *  - A hash function for deterministic pseudorandom gradients,
 *  - A 2D value noise function,
 *  - A fade curve,
 *  - A linear interpolator,
 *  - And an FBM function combining multiple octaves of noise.
 */

#include "noise.h"

#include <stdint.h>
#include <math.h>

static unsigned int Hash_Coords(int x, int y, unsigned int seed);
static double Value_Noise(double x, double y, unsigned int seed);
static double Lerp(double a, double b, double t);
static double Fade(double t);

/**
 * @brief Compute Fractal Brownian Motion (FBM) noise.
 *
 * FBM combines several octaves of noise at increasing frequencies and
 * decreasing amplitudes. Each octave contributes progressively finer
 * detail to the resulting pattern.
 *
 * @param noiseX X-coordinate input to the noise function.
 * @param noiseY Y-coordinate input to the noise function.
 * @param octaveCount Number of FBM octaves to compute.
 * @param lacunarity Frequency multiplier for successive octaves.
 * @param gain Amplitude multiplier for successive octaves.
 * @param seed Seed used for hashing and decorrelating octaves.
 *
 * @return FBM noise value normalized in the approximate range [0, 1].
 */
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
        weightedSum += amplitude * Value_Noise(noiseX * frequency,
                                               noiseY * frequency,
                                               seed + (unsigned int)i * 57u);
        amplitudeSum += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    return weightedSum / amplitudeSum;
}

/**
 * @brief Hash a pair of integer grid coordinates with a seed.
 *
 * This function provides a deterministic pseudo-random value used in
 * value noise generation. Different coordinates and seeds produce unique
 * values with good distribution.
 *
 * @param x Grid X index.
 * @param y Grid Y index.
 * @param seed Noise seed.
 * @return A 32-bit hashed integer value.
 */
static unsigned int Hash_Coords(const int x, const int y, const unsigned int seed) {
    const uint32_t kMulA = 374761393u;
    const uint32_t kMulB = 668265263u;
    const uint32_t kMulC = 1274126177u;

    uint32_t hashValue = (uint32_t)x * kMulA + (uint32_t)y * kMulB + (uint32_t)seed * kMulC;
    hashValue = (hashValue ^ (hashValue >> 13)) * kMulC;
    return hashValue;
}

/**
 * @brief Compute 2D value noise using bilinear interpolation.
 *
 * Steps:
 *  - Determine integer grid cell corners.
 *  - Hash each corner to produce deterministic random values.
 *  - Compute fractional offsets within the cell.
 *  - Apply fade curves for smooth interpolation.
 *  - Blend the values bilinearly.
 *
 * @param x Input X position (floating point).
 * @param y Input Y position (floating point).
 * @param seed Seed used in hashing.
 *
 * @return Noise value in the range [0,1].
 */
static double Value_Noise(const double x, const double y, const unsigned int seed) {
    const int gridX = (int)floor(x);
    const int gridY = (int)floor(y);
    const int gridXNext = gridX + 1;
    const int gridYNext = gridY + 1;

    const double fracX = x - (double)gridX;
    const double fracY = y - (double)gridY;

    const uint32_t kValueMask = 0xFFFFu;
    const double valueNormalizer = kValueMask;

    const double value_top_left  =
        (double)(Hash_Coords(gridX,     gridY,     seed) & kValueMask) / valueNormalizer;
    const double value_top_right =
        (double)(Hash_Coords(gridXNext, gridY,     seed) & kValueMask) / valueNormalizer;
    const double value_bot_left  =
        (double)(Hash_Coords(gridX,     gridYNext, seed) & kValueMask) / valueNormalizer;
    const double value_bot_right =
        (double)(Hash_Coords(gridXNext, gridYNext, seed) & kValueMask) / valueNormalizer;

    const double weight_x = Fade(fracX);
    const double weight_y = Fade(fracY);

    const double interp_top = Lerp(value_top_left,  value_top_right, weight_x);
    const double interp_bot = Lerp(value_bot_left,  value_bot_right, weight_x);

    return Lerp(interp_top, interp_bot, weight_y);
}

/**
 * @brief Linearly interpolate between two values.
 *
 * @param a Start value.
 * @param b End value.
 * @param t Interpolation factor in [0,1].
 *
 * @return a + t * (b - a)
 */
static double Lerp(const double a, const double b, const double t) {
    return a + t * (b - a);
}

/**
 * @brief Fade function used for smoothing interpolation.
 *
 * Implements Ken Perlin’s classic fade curve:
 *     f(t) = 6t^5 - 15t^4 + 10t^3
 *
 * @param t Input value in [0,1].
 * @return Smoothed value.
 */
static double Fade(const double t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
