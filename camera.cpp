#include "camera.h"

void camera::move_local(Vector3D movement) {

	Vector3D global_movement = Vector3D(movement.x * orient.m11 + movement.y * orient.m21 + movement.z * orient.m31,
		movement.x * orient.m12 + movement.y * orient.m22 + movement.z * orient.m32,
		movement.x * orient.m13 + movement.y * orient.m23 + movement.z * orient.m33);

	pos += global_movement;
}

void camera::rotate(Vector3D axis, double angle) {
	orient = orient.rotated(axis, angle);
}