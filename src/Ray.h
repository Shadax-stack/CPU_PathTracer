#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

struct Ray {
	Ray(void) = default;

	glm::vec3 Origin;
	glm::vec3 Direction;
};

#endif