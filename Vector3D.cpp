#include <math.h>
#include "Vector3D.h"

bool Vector3D::operator==(const Vector3D& vec) {
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3D::operator!=(const Vector3D& vec) {
	return !(x == vec.x && y == vec.y && z == vec.z);
}

// arithmetic
Vector3D Vector3D::operator+(const Vector3D& vec) {
	return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

Vector3D Vector3D::operator-(const Vector3D& vec) {
	return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

Vector3D Vector3D::operator*(double val) {
	return Vector3D(x * val, y * val, z * val);
}

Vector3D Vector3D::operator/(double val) {
	assert(val != 0);
	return Vector3D(x / val, y / val, z / val);
}

Vector3D &Vector3D::operator=(const Vector3D& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

Vector3D &Vector3D::operator+=(const Vector3D& vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector3D& Vector3D::operator*=(double val) {
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

Vector3D& Vector3D::operator/=(double val) {
	assert(val != 0);
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

double Vector3D::dot(const Vector3D &vec) {
	return x * vec.x + vec.y * y + vec.z * z;
}

Vector3D Vector3D::cross(const Vector3D& vec) {
    double i = y * vec.z - z * vec.y;
    double j = z * vec.x - x * vec.z;
    double k = x * vec.y - y * vec.x;
    return Vector3D(i, j, k);
}

double Vector3D::mag() {
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalized() {
    assert(mag() != 0);
    return *this / mag();
}