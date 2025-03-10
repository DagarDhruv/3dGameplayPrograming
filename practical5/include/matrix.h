//
// Created by Dhruv Dagar on 10/03/2025.
#ifndef MATRIX3_H
#define MATRIX3_H
#include "math.h"

typedef struct {
    float m[16]; // 4x4 matrix for OpenGL compatibility
} Matrix3;

void Matrix3_identity(Matrix3* matrix);
Matrix3 Matrix3_rotation(float angle);
Matrix3 Matrix3_translate(float dx, float dy, float dz);
Matrix3 Matrix3_scale(float sx, float sy, float sz);
Matrix3 Matrix3_multiply(Matrix3 a, Matrix3 b);

#endif // MATRIX3_H
