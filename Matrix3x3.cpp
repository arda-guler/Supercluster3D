#include "Matrix3x3.h"
#include "Vector3D.h"

Vector3D Matrix3x3::v1() {
	return Vector3D(m11, m12, m13);
}

Vector3D Matrix3x3::v2() {
	return Vector3D(m21, m22, m23);
}

Vector3D Matrix3x3::v3() {
	return Vector3D(m31, m32, m33);
}

Matrix3x3 Matrix3x3::rotated(Vector3D axis, double angle) {
    double q0 = angle;
    double q1 = axis.x;
    double q2 = axis.y;
    double q3 = axis.z;

    double r11 = 2 * (q0 * q0 + q1 * q1) - 1;
    double r12 = 2 * (q1 * q2 - q0 * q3);
    double r13 = 2 * (q1 * q3 + q0 * q2);

    double r21 = 2 * (q1 * q2 + q0 * q3);
    double r22 = 2 * (q0 * q0 + q2 * q2) - 1;
    double r23 = 2 * (q2 * q3 - q0 * q1);

    double r31 = 2 * (q1 * q3 - q0 * q2);
    double r32 = 2 * (q2 * q3 + q0 * q1);
    double r33 = 2 * (q0 * q0 + q3 * q3) - 1;

    Matrix3x3 rot_matrix = Matrix3x3(r11, r12, r13, r21, r22, r23, r31, r32, r33);
    return rot_matrix * *this;
}

Matrix3x3 Matrix3x3::operator*(Matrix3x3 pm) {
    double r11 = m11 * pm.m11 + m12 * pm.m21 + m13 * pm.m31;
    double r12 = m11 * pm.m12 + m12 * pm.m22 + m13 * pm.m32;
    double r13 = m11 * pm.m13 + m12 * pm.m23 + m13 * pm.m33;

    double r21 = m21 * pm.m11 + m22 * pm.m21 + m23 * pm.m31;
    double r22 = m21 * pm.m12 + m22 * pm.m22 + m23 * pm.m32;
    double r23 = m21 * pm.m13 + m22 * pm.m23 + m23 * pm.m33;
        
    double r31 = m31 * pm.m11 + m32 * pm.m21 + m33 * pm.m31;
    double r32 = m31 * pm.m12 + m32 * pm.m22 + m33 * pm.m32;
    double r33 = m31 * pm.m13 + m32 * pm.m23 + m33 * pm.m33;

    return Matrix3x3(r11, r12, r13, r21, r22, r23, r31, r32, r33);
}
