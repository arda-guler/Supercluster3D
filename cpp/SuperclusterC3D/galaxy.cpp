#include "galaxy.h"
#include "constants.h"
#include "cosmicvoid.h"

void galaxy::updatePos(double dt) {
	pos += vel * dt;
}

void galaxy::updateVel(Vector3D accel, double dt) {
	vel += accel * dt;
}

void galaxy::updateStateVectors(Vector3D accel, double dt) {
	updateVel(accel, dt);
	updatePos(dt);
}

bool galaxy::operator==(const galaxy& g) {
	return (pos == g.pos && vel == g.vel && mass == g.mass);
}

bool galaxy::operator!=(const galaxy& g) {
	return !(pos == g.pos && vel == g.vel && mass == g.mass);
}

Vector3D galaxy::getGravityAccelBy(const galaxy& g) {

	if (this->pos == g.pos) { return Vector3D(); }

	Vector3D dir = getDirectionTo(g);
	double d = dist(g);
	if (d >= g.size + this->size) {
		double grav_accel_mag = grav_const * g.mass / (d * d);
		return dir * grav_accel_mag;
	}
	else {
		double clamp_dist = g.size + this->size;
		double grav_accel_mag = grav_const * g.mass / (clamp_dist * clamp_dist);
		return dir * grav_accel_mag;
	}
}

Vector3D galaxy::getDirectionTo(const galaxy& g) {
	return ((pos - g.pos) * -1).normalized();
}

Vector3D galaxy::getVectorTo(const galaxy& g) {
	return (pos - g.pos) * -1;
}
