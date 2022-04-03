#pragma once
#include "Vector3D.h"

class cosmicvoid {
public:
	Vector3D pos;
	double size;

	cosmicvoid() {
		pos = Vector3D();
		size = 1;
	}

	cosmicvoid(Vector3D p, double s) {
		pos = p;
		size = s;
	}
};

