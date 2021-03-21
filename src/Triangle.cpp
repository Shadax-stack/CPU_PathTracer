#include "Triangle.h"
#include "HitInfo.h"

uint32_t& TriangleIndices::operator[](const uint32_t I) {
    return Indices[I];
}

Vertex& Triangle::operator[](const uint32_t I) {
    return Vertices[I];
}

bool Triangle::Intersect(const Ray& Ray, HitInfo& Hit) {
    // this is mostly a copy paste from scratchapixel's code that has been refitted to work with GLM and my own code

    TriangleHitInfo TmpTriangleHitInfo;
    TmpTriangleHitInfo.Triangle = *this;

    glm::vec3 V01 = Vertices[1].Position - Vertices[0].Position;
    glm::vec3 V02 = Vertices[2].Position - Vertices[0].Position;

    glm::vec3 Pvec = glm::cross(Ray.Direction, V02);

    float det = glm::dot(V01, Pvec);

    const float Epsilon = 1e-6f;
    if (fabsf(det) < Epsilon) {
        return false;
    }

    float invDet = 1.0f / det;

    glm::vec3 Tvec = Ray.Origin - Vertices[0].Position;
    TmpTriangleHitInfo.U = glm::dot(Tvec, Pvec) * invDet;
    if (TmpTriangleHitInfo.U < 0 || TmpTriangleHitInfo.U > 1) return false;

    glm::vec3 Qvec = glm::cross(Tvec, V01);
    TmpTriangleHitInfo.V = glm::dot(Ray.Direction, Qvec) * invDet;
    if (TmpTriangleHitInfo.V < 0 || TmpTriangleHitInfo.U + TmpTriangleHitInfo.V  > 1) return false;

    float t = glm::dot(V02, Qvec) * invDet;

    if (t < Hit.Depth) {
        Hit.Depth = t;
        Hit.TriangleHitInfo = TmpTriangleHitInfo;
        return true;
    } else {
        return false;
    }
}