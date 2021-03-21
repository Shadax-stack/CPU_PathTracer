#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(void) : Position(0.0f), Rotation(glm::radians(-90.0f), 0.0f, 0.0f) {}

void Camera::CreateProjection(float FOV, size_t W, size_t H) {
	ProjectionRays.SetSize(W, H);

	AspectRatio = (float)W / (float)H;

	// This is the better solution, I should try to get this to work
	/*
	glm::vec2 FOVInfo;

	FOVInfo.x = FOV * AspectRatio;
	FOVInfo.y = FOV;

	auto RayPrecomputeFunc = [](Texture<glm::vec3>* RayOutput, glm::vec2 FOVData, uint32_t X, uint32_t Y) {
		glm::vec2 LerpCoords = glm::vec2(X, Y) / glm::vec2(RayOutput->GetWidth(), RayOutput->GetHeight());

		glm::vec2 Theta = glm::mix(-FOVData, FOVData, LerpCoords);

		glm::vec3 Direction;

		glm::vec2 SineTheta   = glm::sin(Theta);
		glm::vec2 CosineTheta = glm::cos(Theta);

		Direction.y = SineTheta.y;
		Direction.x = CosineTheta.x * SineTheta.y;
		Direction.z = SineTheta.x * SineTheta.y;

		//Direction.z = -Direction.z;

		glm::normalize(Direction);
		RayOutput->Write(X, Y, Direction); 
	};
	*/

	// Slower and lazier solution, and proj matrix don't work for "non-linear"/"curved" projections
	// Actually on second thought, this is much faster since we avoid sin and cos

	glm::mat4 ProjMatrix = glm::perspective(FOV, AspectRatio, 0.01f, 0.02f);
	glm::mat4 ProjMatrixInv = glm::inverse(ProjMatrix);

	auto RayPrecomputeFunc = [](Texture<glm::vec3>* RayOutput, glm::mat4 ProjMatrixInverse, size_t X, size_t Y) {
		glm::vec2 LerpCoords = (glm::vec2(X, Y) / glm::vec2(RayOutput->GetWidth(), RayOutput->GetHeight())) * 2.0f - 1.0f;	
		glm::vec4 ClipSpace = glm::vec4(LerpCoords, 0.0f, 1.0f);

		glm::vec4 Direction = ProjMatrixInverse * ClipSpace;

		RayOutput->Write(X, Y, glm::normalize(glm::vec3(Direction)));
	};

	ProjectionRays.ForEachPixel<glm::mat4>(RayPrecomputeFunc, ProjMatrixInv);
}

glm::vec3 Camera::GenerateRay(size_t X, size_t Y) {
	// I would add a Camera::Update function to generate the view matrix. Then, I would multiply each sampled ray by the view matrix
	return ViewMatrix * ProjectionRays.Sample(X, Y);
}

void Camera::Update(void) {
	const constexpr float AngleMaxPitch = glm::radians(89.9f);
	Rotation.y = glm::clamp(Rotation.y, -AngleMaxPitch, AngleMaxPitch);

	glm::vec3 Sine   = glm::sin(Rotation);
	glm::vec3 Cosine = glm::cos(Rotation);

	Direction.x = Cosine.y * Cosine.x;
	Direction.y = Sine.y;
	Direction.z = Cosine.y * Sine.x;


	Direction = glm::normalize(Direction);

	ViewMatrix = glm::mat3(glm::lookAt(glm::vec3(0.0f), Direction, glm::vec3(0.0f, -1.0f, 0.0f)));

	Direction.x = -Direction.x;

}

glm::vec3 Camera::GetPosition(void) {
	return Position;
}

void Camera::SetPosition(const glm::vec3& Value) {
	Position = Value;
}

void Camera::AddPosition(const glm::vec3& Value) {
	Position += Value;
}

glm::vec3 Camera::GetDirection(void) {
	return Direction;
}

void Camera::Move(float Distance) {
	AddPosition(Direction * -Distance);
}

glm::vec3 Camera::GetRotation(void) {
	return Rotation;
}

void Camera::SetRotation(const glm::vec3& Value) {
	Rotation = Value;
}

void Camera::AddRotation(const glm::vec3& Value) {
	Rotation += Value;
}