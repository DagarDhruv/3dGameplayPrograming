//
// Created by Dhruv Dagar on 10/03/2025.
//
#include "../include/matrix.h"

void Matrix3_identity(Matrix3* matrix) {
    matrix->m[0] = 1.0f; matrix->m[4] = 0.0f; matrix->m[8] = 0.0f; matrix->m[12] = 0.0f;
    matrix->m[1] = 0.0f; matrix->m[5] = 1.0f; matrix->m[9] = 0.0f; matrix->m[13] = 0.0f;
    matrix->m[2] = 0.0f; matrix->m[6] = 0.0f; matrix->m[10] = 1.0f; matrix->m[14] = 0.0f;
    matrix->m[3] = 0.0f; matrix->m[7] = 0.0f; matrix->m[11] = 0.0f; matrix->m[15] = 1.0f;
}

Matrix3 Matrix3_rotation(float angle) {
    Matrix3 result;
    Matrix3_identity(&result);
    float radians = angle * (3.14159265358979323846f / 180.0f);
    float c = cosf(radians);
    float s = sinf(radians);

    result.m[0] = c;    result.m[8] = s;
    result.m[2] = -s;   result.m[10] = c;

    return result;
}

Matrix3 Matrix3_translate(float dx, float dy, float dz) {
    Matrix3 result;
    Matrix3_identity(&result);

    result.m[12] = dx;
    result.m[13] = dy;
    result.m[14] = dz;

    return result;
}

Matrix3 Matrix3_scale(float sx, float sy, float sz) {
    Matrix3 result;
    Matrix3_identity(&result);

    result.m[0] = sx;
    result.m[5] = sy;
    result.m[10] = sz;

    return result;
}

Matrix3 Matrix3_multiply(Matrix3 a, Matrix3 b) {
    Matrix3 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i + j * 4] =
                    a.m[i + 0 * 4] * b.m[0 + j * 4] +
                    a.m[i + 1 * 4] * b.m[1 + j * 4] +
                    a.m[i + 2 * 4] * b.m[2 + j * 4] +
                    a.m[i + 3 * 4] * b.m[3 + j * 4];
        }
    }

    return result;
}