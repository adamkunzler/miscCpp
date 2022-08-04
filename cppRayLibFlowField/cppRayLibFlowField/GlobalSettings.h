#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

struct GlobalSettings
{
	int screenWidth{ 1024 };
	int screenHeight{ 1024 };

	bool moveZ{ false };

	// particles
	std::size_t numParticles{ 50000 };
	float maxVelocity{ 2.5f };
	unsigned char initTransparency{ 1 };

	// flow field / noise
	std::size_t noiseDim{ 512 };
	
	float noiseScaleX{ (float)screenWidth / (float)noiseDim };
	float noiseScaleHalfX{ noiseScaleX / 2.f };

	float noiseScaleY{ (float)screenHeight / (float)noiseDim };
	float noiseScaleHalfY{ noiseScaleY / 2.f };
	
	float noiseZ{ 0.f };
	
	float noiseScale{ 0.015f };

	float flowFieldStrength{ 0.05f };

	// ---------------------------------------------

	void updateNoiseDim(std::size_t dim)
	{
		noiseDim = dim;
		update();
	}

	void update()
	{
		noiseScaleX = (float)screenWidth / (float)noiseDim;
		noiseScaleY = (float)screenHeight / (float)noiseDim;

		noiseScaleHalfX = noiseScaleX / 2.f;
		noiseScaleHalfY = noiseScaleY / 2.f;
	}

};
#endif