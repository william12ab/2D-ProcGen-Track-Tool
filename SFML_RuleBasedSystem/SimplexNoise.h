#pragma once
#include <math.h>
#include <cstdint> 
static class SimplexNoise
{
public:

	SimplexNoise();
	~SimplexNoise();


	//xpo, ypo = positions
	//2d noise
	//frequency determines the zoom level of the noise.
	static float noise(float xpo, float ypo, float frequency);

	//random seed function 
	int ReturnSeed();
private:

	static int32_t fastFloor(float fp);
};
