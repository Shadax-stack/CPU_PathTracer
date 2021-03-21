#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

class Camera {
public:
	Camera(void);

	void CreateProjection(float FOV, size_t W, size_t H);
	void Update(void);

	glm::vec3 GenerateRay(size_t X, size_t Y);

	glm::vec3 GetPosition(void);
	void SetPosition(const glm::vec3& Value);
	void AddPosition(const glm::vec3& Value);

	glm::vec3 GetRotation(void);
	void SetRotation(const glm::vec3& Value);
	void AddRotation(const glm::vec3& Value);

	glm::vec3 GetDirection(void);

	void Move(float Distance);
private:
	glm::vec3 Position;
	glm::vec3 Rotation;

	glm::vec3 Direction;

	glm::mat3 ViewMatrix;

	float FOV;
	float AspectRatio;
	
	uint32_t Width;
	uint32_t Height;

	Texture<glm::vec3> ProjectionRays;
};