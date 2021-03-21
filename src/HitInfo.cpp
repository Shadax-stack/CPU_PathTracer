#include "HitInfo.h"

Vertex HitInfo::ComputeInterpolatedVertex(void) {
	Vertex InterpolatedVertex =

		TriangleHitInfo.Triangle[1] * TriangleHitInfo.U +
		TriangleHitInfo.Triangle[2] * TriangleHitInfo.V +
		TriangleHitInfo.Triangle[0] * (1.0f - TriangleHitInfo.U - TriangleHitInfo.V)

	;

	InterpolatedVertex.Normal = glm::normalize(InterpolatedVertex.Normal);

	return InterpolatedVertex;
}