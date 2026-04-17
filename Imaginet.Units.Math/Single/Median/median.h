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

#pragma IMAGINET_FRAGMENT_BEGIN "median_i8"

static inline void median_i8(const int8_t* restrict input, int8_t* restrict output, int count)
{
    int8_t temp[count];
    for (int i = 0; i < count; ++i) {
        temp[i] = input[i];
    }

    for (int i = 1; i < count; ++i) {
        int8_t key = temp[i];
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
        *output = (int8_t)(((int16_t)temp[count / 2 - 1] + temp[count / 2]) / 2);
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "median_i16"

static inline void median_i16(const int16_t* restrict input, int16_t* restrict output, int count)
{
    int16_t temp[count];
    for (int i = 0; i < count; ++i) {
        temp[i] = input[i];
    }

    for (int i = 1; i < count; ++i) {
        int16_t key = temp[i];
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
        *output = (int16_t)(((int32_t)temp[count / 2 - 1] + temp[count / 2]) / 2);
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "median_i32"

static inline void median_i32(const int32_t* restrict input, int32_t* restrict output, int count)
{
    int32_t temp[count];
    for (int i = 0; i < count; ++i) {
        temp[i] = input[i];
    }

    for (int i = 1; i < count; ++i) {
        int32_t key = temp[i];
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
        *output = (int32_t)(((int64_t)temp[count / 2 - 1] + temp[count / 2]) / 2);
    }
}

#pragma IMAGINET_FRAGMENT_END