#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "rotate_init_f32"	
																 
static inline int rotate_init_f32(void* restrict rotation_matrix, int rotation_deg, int index)
{																   
    float* matrix = (float*)rotation_matrix;
    
    // Normalize the rotation angle to the range [0, 360) degrees
    float rotation_rad = fmodf(rotation_deg, 360.0f) * (float)M_PI / 180.0f;

    // Create a rotation matrix based on the index variable
    if (index == 0) {
        // Rotation around x-axis
        matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
        matrix[3] = 0.0f; matrix[4] = (float)cosf(rotation_rad); matrix[5] = -(float)sinf(rotation_rad);
        matrix[6] = 0.0f; matrix[7] = (float)sinf(rotation_rad); matrix[8] = (float)cosf(rotation_rad);
    } else if (index == 1) {
        // Rotation around y-axis
        matrix[0] = (float)cosf(rotation_rad); matrix[1] = 0.0f; matrix[2] = (float)sinf(rotation_rad);
        matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
        matrix[6] = -(float)sinf(rotation_rad); matrix[7] = 0.0f; matrix[8] = (float)cosf(rotation_rad);
    } else if (index == 2) {
        // Rotation around z-axis
        matrix[0] = (float)cosf(rotation_rad); matrix[1] = -(float)sinf(rotation_rad); matrix[2] = 0.0f;
        matrix[3] = (float)sinf(rotation_rad); matrix[4] = (float)cosf(rotation_rad); matrix[5] = 0.0f;
        matrix[6] = 0.0f; matrix[7] = 0.0f; matrix[8] = 1.0f;
    }
    
    return 0;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rotate_f32"

static inline void rotate_f32(const float* restrict input, float* restrict rotation_matrix, float* restrict output)
{
    const float* matrix = (const float*)rotation_matrix;
    
    // Apply the rotation matrix to the input vector
    for (int i = 0; i < 3; i++) {
        float sum = 0.0f;
        for (int j = 0; j < 3; j++) {
            sum += matrix[i * 3 + j] * input[j];
        }
        output[i] = sum;
    }
}


#pragma IMAGINET_FRAGMENT_END
