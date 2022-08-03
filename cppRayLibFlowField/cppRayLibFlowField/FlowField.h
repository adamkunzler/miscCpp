#ifndef FLOWFIELD_H
#define FLOWFIELD_H

#include <vector>

#include "raylib.h"
#include "noise/SimplexNoise.h"
#include "GlobalSettings.h"

class FlowField
{
private:
	GlobalSettings _settings;
	RhinoLib::OpenSimplexNoise _noiseGenerator{ 1771L };
	
	RenderTexture2D _texture;

	std::vector<Particle> _particles;

	void initParticles()
	{
		_particles.clear();
		for (std::size_t i = 0; i < _settings.numParticles; ++i)
		{
			auto position = Vector2{ (float)GetRandomValue(0, _settings.screenWidth), (float)GetRandomValue(0, _settings.screenHeight) };

			auto velocity = Vector2{ (float)GetRandomValue(-100, 100), (float)GetRandomValue(-100, 100) };
			velocity = Vector2Normalize(velocity);
			velocity = Vector2Scale(velocity, _settings.maxVelocity);

			auto color = Color{ 255, 255, 255, _settings.initTransparency };

			Particle p{ _settings, position, position, velocity, Vector2{ 0.f, 0.f }, color };
			_particles.push_back(p);
		}
	}

public:

	FlowField(const GlobalSettings& settings) : _settings{ settings } 
	{
		_texture = LoadRenderTexture(_settings.screenWidth, _settings.screenHeight);
	}

	void reset()
	{
		// clear texture to BLACK
		BeginTextureMode(_texture);
		ClearBackground(BLACK);
		EndTextureMode();

		// initialize particles
		initParticles();
	}	

	void update()
	{
		if (_settings.moveZ) _settings.noiseZ += 0.05f;

		// update particles
		for (auto& particle : _particles)
		{
			// set acceleration based on angle of flow field
			auto xIndex = (int)std::floor(particle.position.x / _settings.noiseScaleX);
			auto yIndex = (int)std::floor(particle.position.y / _settings.noiseScaleY);

			if (xIndex < 0) xIndex = 0;
			if (xIndex > _settings.noiseDim - 1) xIndex = _settings.noiseDim - 1;
			if (yIndex < 0) yIndex = 0;
			if (yIndex > _settings.noiseDim - 1) yIndex = _settings.noiseDim - 1;

			auto angle = getNoiseAt(xIndex, yIndex, _settings.noiseZ) * PI * 2.f;
			auto accel = Vector2{ cosf(angle), sinf(angle) };
			//accel = Vector2Normalize(accel);
			particle.acceleration = Vector2Scale(accel, _settings.flowFieldStrength);

			particle.update();
		}
	}

	void preRender()
	{
		// render particles to texture
		BeginTextureMode(_texture);
		BeginBlendMode(BLEND_ADDITIVE);
		
		for (const auto& particle : _particles)
			particle.render();

		EndBlendMode();
		EndTextureMode();
	}

	void render()
	{
		// NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
		DrawTextureRec(_texture.texture, 
			Rectangle{ 0, 0, (float)_texture.texture.width, (float)-_texture.texture.height },
			Vector2{ 0, 0 }, WHITE);
	}

	void renderNoise()
	{
		for (int y = 0; y < _settings.noiseDim; ++y)
		{
			for (int x = 0; x < _settings.noiseDim; ++x)
			{
				auto angle = getNoiseAt(x, y, _settings.noiseZ) * PI * 2.f;

				int cx = x * _settings.noiseScaleX + _settings.noiseScaleHalfX;
				int cy = y * _settings.noiseScaleY + _settings.noiseScaleHalfY;

				int cx2 = cx + _settings.noiseScaleHalfX * cosf(angle);
				int cy2 = cy + _settings.noiseScaleHalfY * sinf(angle);

				DrawLine(cx, cy, cx2, cy2, DARKPURPLE);
				DrawCircle(cx2, cy2, 2.f, DARKGREEN);
			}
		}
	}

	float getNoiseAt(float x, float y, float z)
	{
		float nx = x * _settings.noiseScale;
		float ny = y * _settings.noiseScale;
		float nz = z * _settings.noiseScale;
		return (float)_noiseGenerator.eval(nx, ny, nz);
	}
};

#endif