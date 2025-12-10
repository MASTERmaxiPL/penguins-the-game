#ifndef PENGUINS_THE_GAME_NOISE_H
#define PENGUINS_THE_GAME_NOISE_H

// Generates Fractal Brownian Motion (fBm) noise based on Value Noise.
// noiseX, noiseY  - input noise coordinates
// octaveCount     - number of noise layers added together
// lacunarity      - factor that increases frequency each octave
// gain            - factor that reduces amplitude each octave
// seed            - base seed for deterministic noise generation
double Fbm(double noiseX, double noiseY, int octaveCount, double lacunarity, double gain, unsigned int seed);

#endif
