#pragma once
#include "Vector3D.h"
#include "cosmicvoid.h"

class galaxy {
public:
	double mass;
	Vector3D pos;
	Vector3D vel;
	double size = 0.03;

	galaxy() {
		mass = 100;
		pos = Vector3D();
		vel = Vector3D();
	}

	galaxy(Vector3D p, Vector3D v, double m) {
		mass = m;
		pos = p;
		vel = v;
	}

	inline double dist(const galaxy& g) {
		return (pos - g.pos).mag();
	}
	inline double dist(const cosmicvoid& c) {
		return (pos - c.pos).mag();
	}
	Vector3D getGravityAccelBy(const galaxy& g);
	Vector3D getDirectionTo(const galaxy& g);
	Vector3D getVectorTo(const galaxy& g);

	bool operator==(const galaxy& g);
	bool operator!=(const galaxy& g);

	void updateStateVectors(Vector3D accel, double dt);

private:
	void updateVel(Vector3D accel, double dt);
	void updatePos(double dt);
};

