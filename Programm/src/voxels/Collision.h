#ifndef VOXELS_COLLISION_H_
#define VOXELS_COLLISION_H_

#include <glm/glm.hpp>
using namespace glm;

class Chunk;
class voxel;
class Chunks;
class Camera;

class collision {
public:
	vec3 front;
	vec3 up;
	vec3 right;

	static voxel* getCollision(Chunks* &chunks, Camera* &camera, vec3 dir);


};

#endif VOXELS_COLLISION_H_