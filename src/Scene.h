#ifndef SCENE_H
#define SCENE_H

#include "Ray.h"
#include <vector>

#include "HitInfo.h"
#include "Mesh.h"

class Scene {
public:
	void AddObject(Mesh& Object);
	bool Intersect(const Ray& Ray, HitInfo& Hit);
private:
	std::vector<Mesh> Meshes;
};

#endif