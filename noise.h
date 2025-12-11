#ifndef PENGUINS_THE_GAME_NOISE_H
#define PENGUINS_THE_GAME_NOISE_H

/**
 * @file noise.h
 * @brief Declares the FBM (Fractal Brownian Motion) noise function used for
 *        procedural terrain generation in the penguin game.
 *
 * FBM combines multiple octaves of noise at increasing frequencies and
 * decreasing amplitudes to produce smooth natural-looking patterns.
 */

/**
 * @brief Compute 2D Fractal Brownian Motion (FBM) noise.
 *
 * FBM consists of several layers (octaves) of noise blended together to
 * produce fractal patterns used in procedural map generation.
 *
 * @param noiseX X-coordinate input to the noise function.
 * @param noiseY Y-coordinate input to the noise function.
 * @param octaveCount Number of octaves to compute.
 * @param lacunarity Frequency multiplier for each successive octave.
 * @param gain Amplitude multiplier for each successive octave.
 * @param seed Noise seed for hashing and octave decorrelation.
 *
 * @return A normalized noise value in the approximate range [0, 1].
 */
double Fbm(double noiseX, double noiseY, int octaveCount, double lacunarity, double gain, unsigned int seed);

#endif
