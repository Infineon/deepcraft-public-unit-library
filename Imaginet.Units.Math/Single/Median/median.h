#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "median_f32"

static inline void median_f32(const float* restrict input, float* restrict output, int count) 
{
    float temp[count];
    for (int i = 0; i < count; ++i) {
        temp[i] = input[i];
    }

    // Insertion sort
    for (int i = 1; i < count; ++i) {
        float key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    if (count % 2) {
        *output = temp[count / 2];
    }
    else {
        *output = (temp[count / 2 - 1] + temp[count / 2]) * 0.5f;
    }
}

#pragma IMAGINET_FRAGMENT_END