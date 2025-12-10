#include "noise.h"

#include <stdint.h>
#include <math.h>

// Internal helper functions
static unsigned int Hash_Coords(int x, int y, unsigned int seed);  // Hashes integer coordinates
static double Value_Noise(double x, double y, unsigned int seed);  // Computes interpolated noise value
static double Lerp(double a, double b, double t);                  // Linear interpolation
static double Fade(double t);                                      // Smoothing curve for interpolation

// Performs Fractal Brownian Motion (fBm) noise calculation.
// noiseX, noiseY   - input noise coordinates
// octaveCount      - number of layers added together
// lacunarity       - frequency multiplier per octave
// gain             - amplitude multiplier per octave
// seed             - random seed for noise generation
double Fbm(
    const double noiseX,
    const double noiseY,
    const int octaveCount,
    const double lacunarity,
    const double gain,
    const unsigned int seed
) {
    double amplitude = 1.0;      // Initial amplitude
    double frequency = 1.0;      // Initial frequency
    double weightedSum = 0.0;    // Accumulated weighted values
    double amplitudeSum = 0.0;   // For normalization

    for (int i = 0; i < octaveCount; i++) {
        // Add value noise weighted by amplitude
        weightedSum += amplitude *
            Value_Noise(
                noiseX * frequency,
                noiseY * frequency,
                seed + (unsigned int)i * 57u   // Different seed per octave
            );

        amplitudeSum += amplitude; // Track total amplitude
        amplitude *= gain;         // Reduce amplitude
        frequency *= lacunarity;   // Increase frequency
    }

    // Normalize by total accumulated amplitude
    return weightedSum / amplitudeSum;
}

// Generates a 32-bit hash from integer coordinates (x, y) and seed.
// Used to create deterministic pseudorandom values.
static unsigned int Hash_Coords(const int x, const int y, const unsigned int seed) {
    const uint32_t kMulA = 374761393u;   // Prime multipliers for mixing bits
    const uint32_t kMulB = 668265263u;
    const uint32_t kMulC = 1274126177u;

    uint32_t hashValue =
        (uint32_t)x * kMulA +
        (uint32_t)y * kMulB +
        (uint32_t)seed * kMulC;

    // Final avalanche step for improved randomness
    hashValue = (hashValue ^ (hashValue >> 13)) * kMulC;
    return hashValue;
}

// Computes smooth value noise via bilinear interpolation between hashed grid points.
static double Value_Noise(const double x, const double y, const unsigned int seed) {
    // Determine integer grid cell coordinates
    const int gridX = (int)floor(x);
    const int gridY = (int)floor(y);
    const int gridXNext = gridX + 1;
    const int gridYNext = gridY + 1;

    // Fractional part within the cell
    const double fracX = x - (double)gridX;
    const double fracY = y - (double)gridY;

    const uint32_t kValueMask = 0xFFFFu;      // Mask to reduce hash size
    const double valueNormalizer = kValueMask;

    // Hash values of the 4 surrounding grid corners, mapped to [0, 1]
    const double value_top_left =
        (double)(Hash_Coords(gridX,     gridY,     seed) & kValueMask) / valueNormalizer;
    const double value_top_right =
        (double)(Hash_Coords(gridXNext, gridY,     seed) & kValueMask) / valueNormalizer;
    const double value_bot_left =
        (double)(Hash_Coords(gridX,     gridYNext, seed) & kValueMask) / valueNormalizer;
    const double value_bot_right =
        (double)(Hash_Coords(gridXNext, gridYNext, seed) & kValueMask) / valueNormalizer;

    // Smooth interpolation weights
    const double weight_x = Fade(fracX);
    const double weight_y = Fade(fracY);

    // Interpolate horizontally on top and bottom edges
    const double interp_top = Lerp(value_top_left, value_top_right, weight_x);
    const double interp_bot = Lerp(value_bot_left, value_bot_right, weight_x);

    // Interpolate vertically between the two results
    return Lerp(interp_top, interp_bot, weight_y);
}

// Linear interpolation between values a and b.
static double Lerp(const double a, const double b, const double t) {
    return a + t * (b - a);
}

// Fade curve used for smoothing transitions between noise samples.
// Implements the classical Perlin "fade" function: 6t^5 - 15t^4 + 10t^3
static double Fade(const double t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
