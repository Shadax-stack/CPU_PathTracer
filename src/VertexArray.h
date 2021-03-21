#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Triangle.h"

class VertexArray {
public:

	Triangle InputAssembler(uint32_t Index);
	uint32_t Size(void);

	VertexBuffer VertexBuffer;
	IndexBuffer IndexBuffer;
};