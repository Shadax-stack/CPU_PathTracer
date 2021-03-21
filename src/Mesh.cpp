#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/norm.hpp>

bool QuadraticEquation(const float& a, const float& b, const float& c, float& x0, float& x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5f * b / a;
    else {
        float q = (b > 0) ?
            -0.5f * (b + sqrt(discr)) :
            -0.5f * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

bool IntersectSphere(const Ray& Ray, glm::vec3 Position, float Radius) {
    float t0, t1; // solutions for t if the ray intersects 
     // analytic solution
    glm::vec3 L = Ray.Origin - Position;
    float a = glm::dot(Ray.Direction, Ray.Direction);
    float b = 2 * glm::dot(Ray.Direction, L);
    float c = glm::dot(L, L) - (Radius * Radius);
    if (!QuadraticEquation(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0) return false; // both t0 and t1 are negative 
    }

    return true;
}

bool Mesh::Intersect(const Ray& IntersectRay, HitInfo& Hit) {
#if 1
    Ray BoundingSphereRay = IntersectRay;
    // Move to sphere model space for easier calculations
    BoundingSphereRay.Origin -= BoundingSphere.Position;
    float DistanceFromSphere = glm::length(BoundingSphereRay.Origin);
    // Prevent NaNs and imaginary tangent lengths
    if (DistanceFromSphere > BoundingSphere.Radius) {
        // Compute the length of the tangent
        float TangentLength = glm::sqrt((DistanceFromSphere * DistanceFromSphere) - (BoundingSphere.Radius * BoundingSphere.Radius));
        // Max cosine theta = adjacent leg / hypot = tangent length / hypot length. This will be the maximum value for cos theta an intersecting ray will have
        float CosineThetaMaximumThreshold = TangentLength / DistanceFromSphere;
        // Compute the cosine theta between the direction to the center of the sphere and the ray direction. We need to reverse one of the rays since one is incoming and other is outgoing.
        float CosineTheta = glm::dot(BoundingSphereRay.Origin / DistanceFromSphere, BoundingSphereRay.Direction);
        // Discard intersection tests with this object if the ray does not hit the bounding sphere
        if (CosineTheta <= CosineThetaMaximumThreshold) {
            return false;
        }
    }
#else
    if (!IntersectSphere(IntersectRay, BoundingSphere.Position, BoundingSphere.Radius)) {
        return false;
    }
#endif



    bool Result = false;

    for (uint32_t Index = 0; Index < VertexArray.Size(); Index++) {
        Triangle Triangle = VertexArray.InputAssembler(Index);
        
        Result |= Triangle.Intersect(IntersectRay, Hit);
    }

    // No triangles in mesh
    return Result;
}

void Mesh::LoadMesh(const std::string& File) {
    Assimp::Importer Importer;

    const aiScene* Scene = Importer.ReadFile(File, aiProcess_Triangulate | aiProcess_FlipUVs);

    BoundingSphere.Radius = 0.0f;
    BoundingSphere.Position = glm::vec3(0.0f);

    for (uint32_t MeshIndex = 0; MeshIndex < Scene->mNumMeshes; MeshIndex++) {
        const aiMesh* Mesh = Scene->mMeshes[MeshIndex];
        for (uint32_t VertexIndex = 0; VertexIndex < Mesh->mNumVertices; VertexIndex++) {
            Vertex CurrentVertex;

            CurrentVertex.Position           = glm::vec3(Mesh->mVertices        [VertexIndex].x, Mesh->mVertices        [VertexIndex].y, Mesh->mVertices[VertexIndex].z);
            CurrentVertex.Normal             = glm::vec3(Mesh->mNormals         [VertexIndex].x, Mesh->mNormals         [VertexIndex].y, Mesh->mNormals [VertexIndex].z);
            CurrentVertex.TextureCoordinates = glm::vec2(Mesh->mTextureCoords[0][VertexIndex].x, Mesh->mTextureCoords[0][VertexIndex].y);

            VertexArray.VertexBuffer.AddVertex(CurrentVertex);

            float DistanceFromCenter2 = glm::length2(CurrentVertex.Position);
            if (DistanceFromCenter2 > BoundingSphere.Radius) {
                BoundingSphere.Radius = DistanceFromCenter2;
            }
        }
        for (uint32_t FaceIndex = 0; FaceIndex < Mesh->mNumFaces; FaceIndex++) {
            const aiFace& Face = Mesh->mFaces[FaceIndex];

            TriangleIndexData CurrentIndexData;
            for (uint32_t ElementIndex = 0; ElementIndex < Face.mNumIndices; ElementIndex++) {
                CurrentIndexData[ElementIndex] = Face.mIndices[ElementIndex];
            }

            VertexArray.IndexBuffer.AddTriangleIndexData(CurrentIndexData);
        }
    }

    BoundingSphere.Radius = glm::sqrt(BoundingSphere.Radius);
    printf("Mesh bounding sphere radius: %f\n", BoundingSphere.Radius);

    Importer.FreeScene();
}