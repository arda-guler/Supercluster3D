#pragma once
#include "Vector3D.h"

class Matrix3x3 {
public:
	double m11, m12, m13, m21, m22, m23, m31, m32, m33;

	// constructors
	Matrix3x3() {
		m11 = 1;
		m12 = 0;
		m13 = 0;

		m21 = 0;
		m22 = 1;
		m23 = 0;

		m31 = 0;
		m32 = 0;
		m33 = 1;
	}

	Matrix3x3(double p_m11, double p_m12, double p_m13,
		double p_m21, double p_m22, double p_m23,
		double p_m31, double p_m32, double p_m33) {
		
		m11 = p_m11;
		m12 = p_m12;
		m13 = p_m13;

		m21 = p_m21;
		m22 = p_m22;
		m23 = p_m23;

		m31 = p_m31;
		m32 = p_m32;
		m33 = p_m33;
	}

	Matrix3x3(Vector3D pv1, Vector3D pv2, Vector3D pv3) {
		m11 = pv1.x;
		m12 = pv1.y;
		m13 = pv1.z;

		m21 = pv2.x;
		m22 = pv2.y;
		m23 = pv2.z;

		m31 = pv3.x;
		m32 = pv3.y;
		m33 = pv3.z;
	}

	Matrix3x3(const Matrix3x3& pm) {
		m11 = pm.m11;
		m12 = pm.m12;
		m13 = pm.m13;

		m21 = pm.m21;
		m22 = pm.m22;
		m23 = pm.m23;

		m31 = pm.m31;
		m32 = pm.m32;
		m33 = pm.m33;
	}

	// getting vector forms
	Vector3D v1();
	Vector3D v2();
	Vector3D v3();

	// operations
	Matrix3x3 rotated(Vector3D axis, double angle);
	Matrix3x3 operator*(Matrix3x3 pm);

};