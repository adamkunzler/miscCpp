#define NO_FONT_AWESOME

#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "vendors/rlImGui.h"
#include "vendors/imgui.h"

#include "noise/SimplexNoise.h"

void showInfoOverlay()
{
	bool p_open = true;
	static int corner = 1;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}

	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	if (ImGui::Begin("Info Overlay", &p_open, window_flags))
	{
		ImGui::SetWindowFontScale(1.8f);
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::Separator();

		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

struct Particle
{
	Vector2 position;
	Vector2 oldPosition;
	Vector2 velocity;
	Vector2 acceleration;
	Color color;
};

const int screenWidth{ 2048 };
const int screenHeight{ 1792 };

RhinoLib::OpenSimplexNoise _noiseGenerator{ 1771L };
const std::size_t NOISE_DIM{ 512 };
float noiseScaleX{ screenWidth / NOISE_DIM };
float noiseScaleHalfX{ noiseScaleX / 2.f };
float noiseScaleY{ screenHeight / NOISE_DIM };
float noiseScaleHalfY{ noiseScaleY / 2.f };
float noiseZ{ 0.f };
float _noiseScale{ 0.015f };

float FLOW_FIELD_STRENGTH{ 0.05f };

const std::size_t NUM_PARTICLES{ 50000 };
const float MAX_VELOCITY{ 2.5f };
const unsigned char INIT_TRANSPARENCY{ 1 };

void updateParticle(Particle& p);
void renderParticle(const Particle& p);
Vector2& operator+=(Vector2& lhs, const Vector2& rhs);

void renderNoise();

float getNoiseAt(float x, float y, float z);

void initParticles(std::vector<Particle>& particles);

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	
	InitWindow(screenWidth, screenHeight, ".: RayLib Tesselation :.");

	// create and then clear render texture before main loop
	auto target = LoadRenderTexture(screenWidth, screenHeight);
	BeginTextureMode(target);
	ClearBackground(BLACK);
	EndTextureMode();

	SetTargetFPS(60*1);
		
	// init particles	
	std::vector<Particle> _particles;	
	initParticles(_particles);
	
	bool moveZ{ false };

	// MAIN RENDER LOOP
	//--------------------------------------------------------------------------------------
	rlImGuiSetup(true);
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Process Inputs
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_C))
		{
			// clear the texture
			BeginTextureMode(target);
			ClearBackground(BLACK);
			EndTextureMode();

			initParticles(_particles);
		}

		if (IsKeyPressed(KEY_Z))
		{
			moveZ = !moveZ;
			if (moveZ) std::cout << "Z ON" << std::endl;
			else std::cout << "Z OFF" << std::endl;
		}
		
		//----------------------------------------------------------------------------------


		// Update
		//----------------------------------------------------------------------------------

		if(moveZ) noiseZ += 0.05f;

		// update particles
		for (auto& particle : _particles)
		{
			// set acceleration based on angle of flow field
			auto xIndex = (int)std::floor(particle.position.x / noiseScaleX);
			auto yIndex = (int)std::floor(particle.position.y / noiseScaleY);

			if (xIndex < 0) xIndex = 0;
			if (xIndex > NOISE_DIM - 1) xIndex = NOISE_DIM - 1;
			if (yIndex < 0) yIndex = 0;
			if (yIndex > NOISE_DIM - 1) yIndex = NOISE_DIM - 1;

			auto angle = getNoiseAt(xIndex, yIndex, noiseZ) * PI * 2.f;
			auto accel = Vector2{ cosf(angle), sinf(angle) }; 
			//accel = Vector2Normalize(accel);
			particle.acceleration = Vector2Scale(accel, FLOW_FIELD_STRENGTH);

			updateParticle(particle);
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		
		// render particles to texture
		BeginTextureMode(target);	
		BeginBlendMode(BLEND_ADDITIVE);
		//ClearBackground(BLACK);
		
		for (const auto& particle : _particles)
			renderParticle(particle);

		EndBlendMode();
		EndTextureMode();


		BeginDrawing();
		ClearBackground(BLACK);
				
		// NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
		DrawTextureRec(target.texture, Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height }, Vector2{ 0, 0 }, WHITE);

		rlImGuiBegin();
		showInfoOverlay();
		rlImGuiEnd();

		//renderNoise();
		DrawFPS(10, 10);
		
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------

	rlImGuiShutdown();
	CloseWindow();        // Close window and OpenGL context

	//--------------------------------------------------------------------------------------

	return 0;
}

void initParticles(std::vector<Particle>& particles)
{
	particles.clear();
	for (std::size_t i = 0; i < NUM_PARTICLES; ++i)
	{
		auto position = Vector2{ (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };

		auto velocity = Vector2{ (float)GetRandomValue(-100, 100), (float)GetRandomValue(-100, 100) };
		velocity = Vector2Normalize(velocity);
		velocity = Vector2Scale(velocity, MAX_VELOCITY);

		auto color = Color{ 255, 255, 255, INIT_TRANSPARENCY };

		particles.push_back(Particle{ position, position, velocity, Vector2{ 0.f, 0.f }, color });
	}
}

float getNoiseAt(float x, float y, float z)
{
	float nx = x * _noiseScale;
	float ny = y * _noiseScale;
	float nz = z * _noiseScale;
	return (float)_noiseGenerator.eval(nx, ny, nz);
}

void updateParticle(Particle& p)
{	
	p.oldPosition = p.position;
	p.position += p.velocity;
	p.velocity += p.acceleration;
	p.acceleration = Vector2{ 0.f, 0.f };

	p.velocity = Vector2Normalize(p.velocity);
	p.velocity = Vector2Scale(p.velocity, MAX_VELOCITY);
	
	// window bounds checking/wrapping
	if (p.position.x > screenWidth) {
		p.position.x = 0;
		p.oldPosition = p.position;
	}
	if (p.position.x < 0) {
		p.position.x = screenWidth;
		p.oldPosition = p.position;
	}
	if (p.position.y > screenHeight) {
		p.position.y = 0;
		p.oldPosition = p.position;
	}
	if (p.position.y < 0) {
		p.position.y = screenHeight;
		p.oldPosition = p.position;
	}
}

void renderParticle(const Particle& p)
{
	//DrawCircle(p.position.x, p.position.y, 5.f, p.color);
	DrawLine(p.position.x, p.position.y, p.oldPosition.x, p.oldPosition.y, p.color);
}

Vector2& operator+=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

void renderNoise()
{	
	for (int y = 0; y < NOISE_DIM; ++y)
	{
		for (int x = 0; x < NOISE_DIM; ++x)
		{
			auto angle = getNoiseAt(x, y, noiseZ) * PI * 2.f;

			int cx = x * noiseScaleX + noiseScaleHalfX;
			int cy = y * noiseScaleY + noiseScaleHalfY;

			int cx2 = cx + noiseScaleHalfX * cosf(angle);
			int cy2 = cy + noiseScaleHalfY * sinf(angle);

			DrawLine(cx, cy, cx2, cy2, DARKPURPLE);					
			DrawCircle(cx2, cy2, 2.f, DARKGREEN);
		}
	}
}