#pragma once
#include<assert.h>

class Vector3D {
public:
	double x, y, z;

	// constructors
	Vector3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(double param_x, double param_y, double param_z) {
		x = param_x;
		y = param_y;
		z = param_z;
	}

	Vector3D(const Vector3D& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	// arithmetic
	Vector3D operator+(const Vector3D& vec);
	Vector3D operator-(const Vector3D& vec);
	Vector3D operator*(double val);
	Vector3D operator/(double val);

	Vector3D &operator=(const Vector3D& vec);
	bool operator==(const Vector3D& vec);
	bool operator!=(const Vector3D& vec);

	Vector3D &operator+=(const Vector3D& vec);
	Vector3D &operator-=(const Vector3D& vec);
	Vector3D &operator*=(double val);
	Vector3D &operator/=(double val);

	// vector operations
	double dot(const Vector3D &vec);
	Vector3D cross(const Vector3D& vec);

	double mag();
	Vector3D normalized();

};

