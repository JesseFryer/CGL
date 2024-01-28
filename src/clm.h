/* 
 *
 * clm.h
 *
 * For use in OpenGL projects using C.
 *
 */

#ifndef CLM_H
#define CLM_H

typedef float clmVec2[2];
typedef float clmVec3[3];
typedef float clmVec4[4];
typedef float clmMat3[9];
typedef float clmMat4[16];

#define PI 3.14159265359

/* clm_v2_dot_product
 * ------------------
 * Returns the dot product of v1 and v2.
 */
float clm_v2_dot_product(clmVec2 v1, clmVec2 v2);

/* clm_v3_dot_product
 * ------------------
 * Returns the dot product of v1 and v2.
 */
float clm_v3_dot_product(clmVec3 v1, clmVec3 v2);

/* clm_v2_add
 * ----------
 * Add two vectors, resultant vector is stored in result.
 */
void clm_v2_add(clmVec2 v1, clmVec2 v2, clmVec2 result);

/* clm_v3_add
 * ----------
 * Add two vectors, resultant vector is stored in result.
 */
void clm_v3_add(clmVec3 v1, clmVec3 v2, clmVec3 result);

/* clm_v2_scalar_multiply
 * ----------------------
 * Multiply v by scalar, overwrites vector v.
 */
void clm_v2_scalar_multiply(float scalar, clmVec2 v);

/* clm_v3_scalar_multiply
 * ----------------------
 * Multiply v by scalar, overwrites vector v.
 */
void clm_v3_scalar_multiply(float scalar, clmVec3 v);

/* clm_v2_length
 * -------------
 * Returns the length of vector v.
 */
float clm_v2_length(clmVec2 v);

/* clm_v3_length
 * -------------
 * Returns the length of vector v.
 */
float clm_v3_length(clmVec3 v);

/* clm_v3_cross_product
 * --------------------
 * Stores the orthogonal vector to v1 and v2 in result.
 */
void clm_v3_cross_product(clmVec3 v1, clmVec3 v2, 
        clmVec3 result);

/* clm_v3_normalize
 * ----------------
 * Normalize vector v.
 */
void clm_v3_normalize(clmVec3 v);

/* clm_mat4_mul
 * ------------
 * Compute  mat1 x mat2, stores result in mat1.
 */
void clm_mat4_multiply(clmMat4 mat1, clmMat4 mat2);

/* clm_mat4_multiply_v4
 * --------------------
 * Compute mat x v, stores result in v.
 */
void clm_mat4_multiply_v4(clmMat4 mat, clmVec4 v);

/* clm_mat4_identity
 * -----------------
 * Sets mat to be the identity matrix.
 */
void clm_mat4_identity(clmMat4 mat);

/* clm_mat4_translate
 * ------------------
 * Adds the translation from translate to transform.
 */
void clm_mat4_translate(clmMat4 trans, 
        clmVec3 translate);

/* clm_mat4_rotate
 * ---------------
 * Adds rad radians rotation to trans along the unit 
 * vector axis. axis is assumed to be a unit vector.
 */
void clm_mat4_rotate(clmMat4 trans,
        float rad, clmVec3 axis);

/* clm_mat4_scale
 * --------------
 * Adds a scale transformation to trans.
 */
void clm_mat4_scale(clmMat4 trans, clmVec3 scale);

/* clm_mat4_perspective
 * --------------------
 * Store the perspective projection matrix in proj.
 */
void clm_mat4_perspective(clmMat4 proj, 
        float fov, 
        float aspectRatio, 
        float near, 
        float far);

/* clm_mat4_lookat
 * ---------------
 * Store the lookat view matrix in lookat.
 */
void clm_mat4_lookat(clmMat4 lookat,
        clmVec3 position,
        clmVec3 target,
        clmVec3 up);

/* clm_mat4_print
 * --------------
 * print mat in a readable format.
 */
void clm_mat4_print(clmMat4 mat);

/* clm_vec2_print
 * --------------
 * print v in a readable format.
 */
void clm_v2_print(clmVec2 v);

/* clm_vec3_print
 * --------------
 * print v in a readable format.
 */
void clm_v3_print(clmVec3 v);

/* clm_vec4_print
 * --------------
 * print v in a readable format.
 */
void clm_v4_print(clmVec4 v);

#endif
