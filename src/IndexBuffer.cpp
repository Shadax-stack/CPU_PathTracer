#include "IndexBuffer.h"

uint32_t& TriangleIndexData::operator[](uint32_t Index) {
	return Indices[Index];
}

TriangleIndexData IndexBuffer::operator[](uint32_t Index) {
	return Indices[Index];
}

void IndexBuffer::AddTriangleIndexData(const TriangleIndexData& IndexData) {
	Indices.push_back(IndexData);
}

uint32_t IndexBuffer::Size(void) {
	return Indices.size();
}