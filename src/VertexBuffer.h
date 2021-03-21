#pragma once

#include <vector>
#include <stdint.h>

#include "Vertex.h"

class VertexBuffer {
public:
	Vertex operator[](uint32_t Index);
	void AddVertex(const Vertex& Vertex);
private:
	std::vector<Vertex> Vertices;
};