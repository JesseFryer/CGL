#include "clm.h"
#include <math.h>

float clm_v2_dot_product(clmVec2 v1, clmVec2 v2) {
    return (v1[0] * v2[0]) + (v1[1] * v2[1]);
}

float clm_v3_dot_product(clmVec3 v1, clmVec3 v2) {
    return (v1[0] * v2[0]) + (v1[1] * v2[1]) + 
        (v1[2] * v2[2]);
}

void clm_v2_add(clmVec2 v1, clmVec2 v2, clmVec2 result) {
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
}

void clm_v3_add(clmVec3 v1, clmVec3 v2, clmVec3 result) {
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
    result[2] = v1[2] + v2[2];
}

void clm_v2_scalar_multiply(float scalar, clmVec2 v) {
    v[0] *= scalar;
    v[1] *= scalar;
}

void clm_v3_scalar_multiply(float scalar, clmVec3 v) {
    v[0] *= scalar;
    v[1] *= scalar;
    v[2] *= scalar;
}

float clm_v2_length(clmVec2 v) {
    return sqrtf(
            (v[0] * v[0]) +
            (v[1] * v[1]));
}

float clm_v3_length(clmVec3 v) {
    return sqrtf(
            (v[0] * v[0]) + 
            (v[1] * v[1]) +
            (v[2] * v[2]));
}

void clm_v3_cross_product(clmVec3 v1, clmVec3 v2, 
        clmVec3 result) {
    result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

void clm_v3_normalize(clmVec3 v) {
    float len = clm_v3_length(v);
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

void clm_mat4_multiply(clmMat4 mat1, clmMat4 mat2) {
    // Compute the matrix multiplication into tmp.
    clmMat4 tmp;
    for (int i = 0; i < 16; i++) {
        int mat1Idx = i % 4;
        int mat2Idx = (i / 4) * 4;
        tmp[i] = 
            (mat1[mat1Idx + 0]  * mat2[mat2Idx + 0]) +
            (mat1[mat1Idx + 4]  * mat2[mat2Idx + 1]) +
            (mat1[mat1Idx + 8]  * mat2[mat2Idx + 2]) +
            (mat1[mat1Idx + 12] * mat2[mat2Idx + 3]);
    }

    // Copy over to mat1.
    for (int i = 0; i < 16; i++) {
        mat1[i] = tmp[i];
    }
}

void clm_mat4_multiply_v4(clmMat4 mat, clmVec4 v) {
    // Compute multiplication into tmp.
    clmVec4 tmp;
    for  (int i = 0; i < 4; i++) {
        tmp[i] =
            (mat[0  + i]  * v[0]) + 
            (mat[4  + i]  * v[1]) +
            (mat[8  + i]  * v[2]) +
            (mat[12 + i]  * v[3]);
    }

    // Copy over into v.
    for (int i = 0; i < 4; i++) {
        v[i] = tmp[i];
    }
}

void clm_mat4_identity(clmMat4 mat) {
    mat[0] = 1.0f;  
    mat[1] = 0.0f;  
    mat[2] = 0.0f;  
    mat[3] = 0.0f;

    mat[4] = 0.0f; 
    mat[5] = 1.0f;
    mat[6] = 0.0f; 
    mat[7] = 0.0f;

    mat[8]  = 0.0f; 
    mat[9]  = 0.0f; 
    mat[10] = 1.0f; 
    mat[11] = 0.0f;

    mat[12] = 0.0f; 
    mat[13] = 0.0f; 
    mat[14] = 0.0f; 
    mat[15] = 1.0f;
}

void clm_mat4_translate(clmMat4 transform,
        clmVec3 trans) {
    clmMat4 translateMat = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        trans[0], trans[1], trans[2], 1.0f
    };
    clm_mat4_multiply(transform, translateMat);
}

void clm_mat4_rotate(clmMat4 trans,
        float rad, clmVec3 axis) {
    float rotX = axis[0];
    float rotY = axis[1];
    float rotZ = axis[2];
    float sinTheta = sinf(rad);
    float cosTheta = cosf(rad);
    float justCoz = 1 - cosTheta; // This is reused a lot.

    clmMat4 rotMat = {
        cosTheta + (rotX * rotX * justCoz),
        (rotY * rotX * justCoz) + (rotZ * sinTheta),
        (rotZ * rotX * justCoz) - (rotY * sinTheta),
        0,

        (rotX * rotY * justCoz) - (rotZ * sinTheta),
        cosTheta + (rotY * rotY * justCoz),
        (rotZ * rotY * justCoz) + (rotX * sinTheta),
        0,

        (rotX * rotZ * justCoz) + (rotY * sinTheta),
        (rotY * rotZ * justCoz) - (rotX * sinTheta),
        cosTheta + (rotZ * rotZ * justCoz),
        0,

        0,
        0,
        0,
        1
    };

    clm_mat4_multiply(trans, rotMat);
}

void clm_mat4_scale(clmMat4 trans, clmVec3 scale) {
    clmMat4 scaleMat = {
        scale[0], 0, 0, 0,
        0, scale[1], 0, 0,
        0, 0, scale[2], 0,
        0, 0, 0, 1
    };

    clm_mat4_multiply(trans, scaleMat);
}

void clm_mat4_perspective(clmMat4 proj, 
        float fov, 
        float aspectRatio, 
        float near, 
        float far) {
    // Yanked this from:
    //     Learn WebGL 8.3 Perspective Projections.

    // We need left, right, top and bottom of frustum.
    float top = near * tanf(fov * 0.5);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;
    
    // Now lets a go.
    // 0 it all out first.
    for (int i = 0; i < 16; i++) {
        proj[i] = 0.0f;
    }

    // Place all the bits in there.
    proj[0] = (2.0f * near) / (right - left);
    proj[5] = (2.0f * near) / (top - bottom);
    proj[10] = (-(far + near)) / (far - near);
    proj[11] = -1.0f;
    proj[12] = (-near * (right + left)) / (right - left);
    proj[13] = (-near * (top + bottom)) / (top - bottom);
    proj[14] = (2.0f * far * near) / (near - far);
}

void clm_mat4_lookat(clmMat4 lookat,
        clmVec3 position,
        clmVec3 target,
        clmVec3 up) {
    // Get the camera face vector.
    clmVec3 camDir;
    clm_v3_scalar_multiply(-1.0f, target);
    clm_v3_add(position, target, camDir);
    clm_v3_normalize(camDir);

    // Get the right vector.
    clmVec3 camRight;
    clm_v3_cross_product(up, camDir, camRight);
    clm_v3_normalize(camRight);

    // Get the camera up vector.
    clmVec3 camUp;
    clm_v3_cross_product(camDir, camRight, camUp);

    // Multiply these two together after setting them up.
    clmMat4 positionTrans;
    clm_mat4_identity(positionTrans);
    clm_mat4_identity(lookat);

    lookat[0] = camRight[0];
    lookat[4] = camRight[1];
    lookat[8] = camRight[2];

    lookat[1] = camUp[0];
    lookat[5] = camUp[1];
    lookat[9] = camUp[2];

    lookat[2]  = camDir[0];
    lookat[6]  = camDir[1];
    lookat[10] = camDir[2];

    positionTrans[12] = -position[0];
    positionTrans[13] = -position[1];
    positionTrans[14] = -position[2];

    clm_mat4_multiply(lookat, positionTrans);
}

void clm_mat4_print(clmMat4 mat) {
    for (int i = 0; i < 4; i++) {
        // Print each row.
        for (int j = 0; j < 4; j++) {
            printf("%.2f ", mat[i + (j * 4)]);
        }
        printf("\n");
    }
}

void clm_v2_print(clmVec2 v) {
    printf("(%.2f, %.2f)", v[0], v[1]);
}

void clm_v3_print(clmVec3 v) {
    printf("(%.2f, %.2f, %.2f)", v[0], v[1], v[2]);
}

void clm_v4_print(clmVec4 v) {
    printf("(%.2f, %.2f, %.2f, %.2f)",
            v[0], v[1], v[2], v[3]);
}
