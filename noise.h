#ifndef PENGUINS_THE_GAME_NOISE_H
#define PENGUINS_THE_GAME_NOISE_H

double Fbm(double noiseX, double noiseY, int octaveCount, double lacunarity, double gain, unsigned int seed);
static unsigned int Hash_Coords(int x, int y, unsigned int seed);
static double Value_Noise(double x, double y, unsigned int seed);
static double Lerp(double a, double b, double t);
static double Fade(double t);


#endif