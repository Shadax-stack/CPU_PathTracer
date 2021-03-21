#pragma once

#include "Vertex.h"
#include <vector>
#include <glm/gtx/intersect.hpp>
#include "Triangle.h"
#include "HitInfo.h"
#include "Vertex.h"
#include "VertexArray.h"
#include <string>

class Mesh {
public:
	VertexArray VertexArray;
	struct {
		glm::vec3 Position;
		float Radius;
	} BoundingSphere;
	bool Intersect(const Ray& Ray, HitInfo& Hit);

	void LoadMesh(const std::string& File);
};