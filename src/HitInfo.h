#pragma once

#include <float.h>

#include "Triangle.h"

struct TriangleHitInfo {
	TriangleHitInfo(void) = default;

	Triangle Triangle;
	float U, V;
};

struct HitInfo {
	float Depth;
	TriangleHitInfo TriangleHitInfo;
	HitInfo(void) : Depth(FLT_MAX) {}
	Vertex ComputeInterpolatedVertex(void);
};
