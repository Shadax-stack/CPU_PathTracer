#pragma once

#include <vector>
#include <stdint.h>

struct TriangleIndexData {
	uint32_t Indices[3];
	uint32_t& operator[](uint32_t Index);
};

class IndexBuffer {
public:
	TriangleIndexData operator[](uint32_t Index);

	void AddTriangleIndexData(const TriangleIndexData& IndexData);
	uint32_t Size(void);
private:
	std::vector<TriangleIndexData> Indices;
	uint32_t CurrentTriangle;
};