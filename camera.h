#pragma once
#include "Vector3D.h"
#include "Matrix3x3.h"
#include "galaxy.h"
#include "cosmicvoid.h"

class camera {
public:
	Vector3D pos;
	Matrix3x3 orient;

	// constructors
	camera() {
		pos = Vector3D();
		orient = Matrix3x3();
	}

	camera(Vector3D ppos) {
		pos = ppos;
	}

	camera(Matrix3x3 porient) {
		orient = porient;
	}

	camera(Vector3D ppos, Matrix3x3 porient) {
		pos = ppos;
		orient = porient;
	}

	// operations
	inline double dist(const galaxy& g) {
		return (pos - g.pos).mag();
	}
	inline double dist(const cosmicvoid& c) {
		return (pos - c.pos).mag();
	}
	inline double dist(const Vector3D& vec) {
		return (pos - vec).mag();
	}

	void move_local(Vector3D movement);
	void rotate(Vector3D axis, double angle);

};
