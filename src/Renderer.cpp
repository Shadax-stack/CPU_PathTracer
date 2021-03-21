#include "Renderer.h"
#include "PresentUtil.h"
#include <glm/gtc/random.hpp>

const int SamplesPerPixel = 4096;

void Renderer::Init(size_t Width, size_t Height) {
	BackBuffer.Create(Width, Height);

	InitRendererResources(&BackBuffer, &ScreenTexture);

	Camera.CreateProjection(glm::radians(45.0f), Width, Height);
}

void Renderer::Free(void) {
	BackBuffer.Free();
}

void Renderer::UpdateScreen(void) {
	// TODO: multithread this
	auto ImageDrawFunc = [](Texture<ColorRGB_32F>* Tex, Renderer* Renderer, size_t X, size_t Y) {
		Renderer->ComputeColor(X, Y);
	};

	BackBuffer.ForEachPixel<Renderer*>(ImageDrawFunc, this);
}

void Renderer::ComputeColor(size_t X, size_t Y) {
	Ray ViewRay;
	ViewRay.Direction = Camera.GenerateRay(X, Y);

	HitInfo Hit;

	ViewRay.Origin = Camera.GetPosition();

	if (World.Intersect(ViewRay, Hit)) {
		BackBuffer.Write(X, Y, ShadeHit(Hit));
	} else {
		glm::vec3 Color = ComputeIncomingRadiance(ViewRay);
		ColorRGB_32F ColorRGB32F_Write(Color.r, Color.g, Color.b);
		BackBuffer.Write(X, Y, ColorRGB32F_Write);
	}
}

ColorRGB_32F Renderer::ShadeHit(HitInfo& Hit) {
	Vertex CurrentVertex = Hit.ComputeInterpolatedVertex();

	glm::vec3 Tangent = glm::cross(CurrentVertex.Normal, glm::vec3(0.0f, 1.0f, 0.0f));
	float TangentLength = glm::length(Tangent);
	if (TangentLength < 1e-3f) {
		Tangent = glm::cross(CurrentVertex.Normal, glm::vec3(0.0f, 0.0f, 1.0f));
		float TangentLength = glm::length(Tangent);
	}
	Tangent /= TangentLength;
	glm::mat3 TBN = glm::mat3(Tangent, glm::cross(Tangent, CurrentVertex.Normal), CurrentVertex.Normal);

	glm::vec3 Color(0.0f);

	// Compute the color
	const float PDF = 1.0f / (2.0f * MATH_PI);

	Ray RadianceRay;
	RadianceRay.Origin = CurrentVertex.Position;
	for (uint32_t Sample = 0; Sample < SamplesPerPixel; Sample++) {
		glm::vec3 RandomDirection = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
		RandomDirection = glm::normalize(RandomDirection);
		RadianceRay.Direction = TBN * RandomDirection;
		glm::vec3 IncomingRadiance = ComputeIncomingRadiance(RadianceRay);
		float BRDF = glm::dot(CurrentVertex.Normal, RadianceRay.Direction) / MATH_PI;
		glm::vec3 Integrand = BRDF * IncomingRadiance / PDF;
		Color += Integrand;
	}

	Color /= SamplesPerPixel;

	ColorRGB_32F ColorRGB32F_Write(Color.r, Color.g, Color.b);
	return ColorRGB32F_Write;
}

glm::vec3 Renderer::ComputeIncomingRadiance(const Ray& Ray) {
	float UpDot = glm::dot(Ray.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	// Sky by builderboy
	const glm::vec3 Coefficients = glm::vec3(0.0625, 0.125, 0.25);
	glm::vec3 Color = Coefficients / (UpDot * UpDot + Coefficients);
	return Color;
}

void Renderer::AddMesh(Mesh* Mesh) {
	World.AddObject(*Mesh);
}

void Renderer::Present(void) {
	PresentImage(&BackBuffer, ScreenTexture);
}

void Renderer::Begin(void) {
	ColorRGB_32F DefaultColor = ColorRGB_32F(0.0f, 0.0f, 0.0f);

	BackBuffer.Clear(DefaultColor);
}

void Renderer::End(void) {
	Present();
}

Camera* Renderer::GetCamera(void) {
	return &Camera;
}