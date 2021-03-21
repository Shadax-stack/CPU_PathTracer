#include "Scene.h"

void Scene::AddObject(Mesh& Object) {
	Meshes.push_back(Object);
}
bool Scene::Intersect(const Ray& Ray, HitInfo& Hit) {
	bool Result = false;

	for (Mesh& Mesh : Meshes) {
		Result |= Mesh.Intersect(Ray, Hit);
	}

	return Result;
}