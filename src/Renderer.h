#pragma once

#include "Scene.h"
#include "Texture.h"
#include "Color.h"
#include "Camera.h"

#define MATH_PI 3.1415926535f

class Renderer {
public:
	void Init(size_t Width, size_t Height);
	void Free(void);

	void Begin(void);
	void End(void);

	void UpdateScreen(void);

	void AddMesh(Mesh* Mesh);

	Camera* GetCamera(void);
private:
	void Present(void);

	void ComputeColor(size_t X, size_t Y);
	ColorRGB_32F ShadeHit(HitInfo& Hit);

	glm::vec3 ComputeIncomingRadiance(const Ray& Ray);

	Scene World;

	unsigned int ScreenTexture;
	Texture<ColorRGB_32F> BackBuffer;

	Camera Camera;
};