#include "Collision.h"
#include "Chunks.h"
#include "Chunk.h"
#include "voxel.h"
#include "../window/camera.h"
#include <iostream>

#include <glm/glm.hpp>
using namespace glm;

voxel* collision::getCollision(Chunks* &chunks, Camera* &camera, vec3 dir) {
	vec3 temp;
	voxel* vox = chunks->rayCast(camera->position, dir, 1.0f, temp, temp, temp);
	if (vox != nullptr)
		return vox;
	else
		return nullptr;
}
