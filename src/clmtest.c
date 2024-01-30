#include "clm.h"
#include <stdio.h>
#include <math.h>

int main() {
    clmVec2 v1 = { 1.0f, 2.0f };
    clmVec2 v2 = { 0.5f, -0.5f };
    clmVec3 v3 = { 1.0f, 3.0f, -2.0f };
    clmVec3 v4 = { 4.0f, -2.0f, 5.0f };

    printf("v1: ");
    clm_v2_print(v1);
    printf("\nv2: ");
    clm_v2_print(v2);
    printf("\nv3: ");
    clm_v3_print(v3);
    printf("\nv4: ");
    clm_v3_print(v4);

    printf("\n\nDot Product of v1 and v2:\n%.2f\n", 
            clm_v2_dot_product(v1, v2));
    printf("Dot Product of v3 and v4:\n%.2f\n", 
            clm_v3_dot_product(v3, v4));

    clmMat4 trans;
    clm_mat4_identity(trans);
    printf("\nIdentity:\n");
    clm_mat4_print(trans);
    clmVec4 vec = { 1.0f, 0.0f, 0.0f, 1.0f };
    clmVec3 translate = { 1.0f, 1.0f, 0.0f };
    printf("\nvec before translation:\n");
    clm_v4_print(vec);
    clm_mat4_translate(trans, translate);
    clm_mat4_multiply_v4(trans, vec);
    printf("\nvec after translation:\n");
    clm_v4_print(vec);

    clmVec2 res1;
    clmVec3 res2;
    clm_v2_add(v1, v2, res1);
    clm_v3_add(v3, v4, res2);
    printf("\n\nv1 + v2:\n");
    clm_v2_print(res1);
    printf("\n\nv3 + v4:\n");
    clm_v3_print(res2);

    printf("\nLength of v1:\n%.2f\n", clm_v2_length(v1));

    clmMat4 m1 = {
        1, 2, 3, 5, 3, -8, 3, 8, 2, 7, -2, 0, 1, 6, 5, 1
    };
    
    clmMat4 m2 = {
        0, 2, 7, 5, 4, 8, 9, 4, -2, 7, 2, -10, 1, -3, 5, 1
    };

    clm_mat4_multiply(m1, m2);

    printf("\nm1 x m2:\n");
    clm_mat4_print(m1);

    printf("\nnot normlized:\n");
    clmVec3 v = { 2.0f, 0.5f, 7.0f };
    clm_v3_print(v);
    clm_v3_normalize(v);
    printf("\nnormlized:\n");
    clm_v3_print(v);


    printf("sqrt(4): %f\n", sqrt(4));

    return 0;
}
