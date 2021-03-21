#include "VertexArray.h"

Triangle VertexArray::InputAssembler(uint32_t Index) {
	TriangleIndexData TriangleIndices = IndexBuffer[Index];

	Triangle Triangle;

	Triangle[0] = VertexBuffer[TriangleIndices[0]];
	Triangle[1] = VertexBuffer[TriangleIndices[1]];
	Triangle[2] = VertexBuffer[TriangleIndices[2]];

	return Triangle;
}

uint32_t VertexArray::Size(void) {
	return IndexBuffer.Size();
}