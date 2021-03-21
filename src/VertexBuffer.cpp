#include "VertexBuffer.h"

Vertex VertexBuffer::operator[](uint32_t Index) {
	return Vertices[Index];
}

void VertexBuffer::AddVertex(const Vertex& Vertex) {
	Vertices.push_back(Vertex);
}