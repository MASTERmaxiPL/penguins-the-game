#ifndef NOISE_H
#define NOISE_H

/**
 * @file noise.h
 * @brief Procedural noise functions used by board generation (FBM / value noise).
 */

/**
 * @brief Fractional Brownian Motion noise (FBM) using value noise base.
 *
 * @param noiseX X coordinate in noise space.
 * @param noiseY Y coordinate in noise space.
 * @param octaveCount Number of octaves to combine.
 * @param lacunarity Frequency multiplier per octave.
 * @param gain Amplitude multiplier per octave.
 * @param seed Seed value to make noise deterministic.
 * @return Noise value (double) in approximate range [-1..1] or 0..1 depending on implementation.
 */
double Fbm(double noiseX, double noiseY, int octaveCount, double lacunarity, double gain, unsigned int seed);

#endif
