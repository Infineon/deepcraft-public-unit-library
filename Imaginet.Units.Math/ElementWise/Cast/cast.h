#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <float.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

// Float32 to other types - Clamp mode
#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i8_clamp"
static inline void cast_f32_to_i8_clamp(const float* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to int8 range and round
        float val = roundf(input[i]);
        if (val > 127.0f) val = 127.0f;
        else if (val < -128.0f) val = -128.0f;
        output[i] = (int8_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i8_range_0_1"
static inline void cast_f32_to_i8_range_0_1(const float* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [-128, 127]
        // First clamp to [0,1], then map to signed range
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (int8_t)roundf(normalized * 255.0f - 128.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i8_range_neg1_1"
static inline void cast_f32_to_i8_range_neg1_1(const float* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [-128, 127]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (int8_t)roundf(normalized * 127.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i16_clamp"
static inline void cast_f32_to_i16_clamp(const float* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to int16 range and round
        float val = roundf(input[i]);
        if (val > 32767.0f) val = 32767.0f;
        else if (val < -32768.0f) val = -32768.0f;
        output[i] = (int16_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i16_range_0_1"
static inline void cast_f32_to_i16_range_0_1(const float* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [-32768, 32767]
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (int16_t)roundf(normalized * 65535.0f - 32768.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i16_range_neg1_1"
static inline void cast_f32_to_i16_range_neg1_1(const float* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [-32768, 32767]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (int16_t)roundf(normalized * 32767.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i32_clamp"
static inline void cast_f32_to_i32_clamp(const float* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to int32 range and round
        float val = roundf(input[i]);
        if (val > 2147483647.0f) val = 2147483647.0f;
        else if (val < -2147483648.0f) val = -2147483648.0f;
        output[i] = (int32_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i32_range_0_1"
static inline void cast_f32_to_i32_range_0_1(const float* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [-2147483648, 2147483647]
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (int32_t)roundf(normalized * 4294967295.0f - 2147483648.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_i32_range_neg1_1"
static inline void cast_f32_to_i32_range_neg1_1(const float* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [-2147483648, 2147483647]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (int32_t)roundf(normalized * 2147483647.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u8_clamp"
static inline void cast_f32_to_u8_clamp(const float* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to uint8 range and round
        float val = roundf(input[i]);
        if (val > 255.0f) val = 255.0f;
        else if (val < 0.0f) val = 0.0f;
        output[i] = (uint8_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u8_range_0_1"
static inline void cast_f32_to_u8_range_0_1(const float* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [0, 255]
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (uint8_t)roundf(normalized * 255.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u8_range_neg1_1"
static inline void cast_f32_to_u8_range_neg1_1(const float* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [0, 255]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (uint8_t)roundf((normalized + 1.0f) * 127.5f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u16_clamp"
static inline void cast_f32_to_u16_clamp(const float* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to uint16 range and round
        float val = roundf(input[i]);
        if (val > 65535.0f) val = 65535.0f;
        else if (val < 0.0f) val = 0.0f;
        output[i] = (uint16_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u16_range_0_1"
static inline void cast_f32_to_u16_range_0_1(const float* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [0, 65535]
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (uint16_t)roundf(normalized * 65535.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u16_range_neg1_1"
static inline void cast_f32_to_u16_range_neg1_1(const float* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [0, 65535]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (uint16_t)roundf((normalized + 1.0f) * 32767.5f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u32_clamp"
static inline void cast_f32_to_u32_clamp(const float* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Clamp to uint32 range and round
        float val = roundf(input[i]);
        if (val > 4294967295.0f) val = 4294967295.0f;
        else if (val < 0.0f) val = 0.0f;
        output[i] = (uint32_t)val;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u32_range_0_1"
static inline void cast_f32_to_u32_range_0_1(const float* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0.0, 1.0] to [0, 4294967295]
        float normalized = fmaxf(0.0f, fminf(1.0f, input[i]));
        output[i] = (uint32_t)roundf(normalized * 4294967295.0f);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_f32_to_u32_range_neg1_1"
static inline void cast_f32_to_u32_range_neg1_1(const float* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-1.0, 1.0] to [0, 4294967295]
        float normalized = fmaxf(-1.0f, fminf(1.0f, input[i]));
        output[i] = (uint32_t)roundf((normalized + 1.0f) * 2147483647.5f);
    }
}
#pragma IMAGINET_FRAGMENT_END

// Int8 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_f32_clamp"
static inline void cast_i8_to_f32_clamp(const int8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_f32_range_0_1"
static inline void cast_i8_to_f32_range_0_1(const int8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-128,127] to [0,1]
        output[i] = ((float)input[i] + 128.0f) / 255.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_f32_range_neg1_1"
static inline void cast_i8_to_f32_range_neg1_1(const int8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-128,127] to [-1,1]
        output[i] = (float)input[i] / 127.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_i16"
static inline void cast_i8_to_i16(const int8_t* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int16_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_i32"
static inline void cast_i8_to_i32(const int8_t* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_u8"
static inline void cast_i8_to_u8(const int8_t* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-128,127] to [0,255]
        output[i] = (uint8_t)((int)input[i] + 128);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_u16"
static inline void cast_i8_to_u16(const int8_t* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-128,127] to [0,255], then extend to uint16
        output[i] = (uint16_t)((int)input[i] + 128);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i8_to_u32"
static inline void cast_i8_to_u32(const int8_t* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-128,127] to [0,255], then extend to uint32
        output[i] = (uint32_t)((int)input[i] + 128);
    }
}
#pragma IMAGINET_FRAGMENT_END

// Int16 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_f32_clamp"
static inline void cast_i16_to_f32_clamp(const int16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_f32_range_0_1"
static inline void cast_i16_to_f32_range_0_1(const int16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [0,1]
        output[i] = ((float)input[i] + 32768.0f) / 65535.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_f32_range_neg1_1"
static inline void cast_i16_to_f32_range_neg1_1(const int16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [-1,1]
        output[i] = (float)input[i] / 32767.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_i8"
static inline void cast_i16_to_i8(const int16_t* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [-128,127]
        // Scale down from 16-bit to 8-bit range
        output[i] = (int8_t)(input[i] >> 8);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_i32"
static inline void cast_i16_to_i32(const int16_t* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_u8"
static inline void cast_i16_to_u8(const int16_t* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [0,65535], then scale to [0,255]
        uint16_t temp = (uint16_t)((int32_t)input[i] + 32768);
        output[i] = (uint8_t)(temp >> 8);  // Scale down to 8-bit
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_u16"
static inline void cast_i16_to_u16(const int16_t* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [0,65535]
        output[i] = (uint16_t)((int32_t)input[i] + 32768);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i16_to_u32"
static inline void cast_i16_to_u32(const int16_t* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-32768,32767] to [0,65535], then extend to uint32
        output[i] = (uint32_t)((int32_t)input[i] + 32768);
    }
}
#pragma IMAGINET_FRAGMENT_END

// Int32 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_f32_clamp"
static inline void cast_i32_to_f32_clamp(const int32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_f32_range_0_1"
static inline void cast_i32_to_f32_range_0_1(const int32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [0,1]
        output[i] = ((double)input[i] + 2147483648.0) / 4294967295.0;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_f32_range_neg1_1"
static inline void cast_i32_to_f32_range_neg1_1(const int32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [-1,1]
        output[i] = (double)input[i] / 2147483647.0;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_i8"
static inline void cast_i32_to_i8(const int32_t* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [-128,127]
        // Scale down from 32-bit to 8-bit range
        output[i] = (int8_t)(input[i] >> 24);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_i16"
static inline void cast_i32_to_i16(const int32_t* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [-32768,32767]
        // Scale down from 32-bit to 16-bit range
        output[i] = (int16_t)(input[i] >> 16);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_u8"
static inline void cast_i32_to_u8(const int32_t* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [0,4294967295], then scale to [0,255]
        uint32_t temp = (uint32_t)((int64_t)input[i] + 2147483648LL);
        output[i] = (uint8_t)(temp >> 24);  // Scale down to 8-bit
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_u16"
static inline void cast_i32_to_u16(const int32_t* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [0,4294967295], then scale to [0,65535]
        uint32_t temp = (uint32_t)((int64_t)input[i] + 2147483648LL);
        output[i] = (uint16_t)(temp >> 16);  // Scale down to 16-bit
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_i32_to_u32"
static inline void cast_i32_to_u32(const int32_t* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [-2147483648,2147483647] to [0,4294967295]
        output[i] = (uint32_t)((int64_t)input[i] + 2147483648LL);
    }
}
#pragma IMAGINET_FRAGMENT_END

// UInt8 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_f32_clamp"
static inline void cast_u8_to_f32_clamp(const uint8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_f32_range_0_1"
static inline void cast_u8_to_f32_range_0_1(const uint8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,255] to [0,1]
        output[i] = (float)input[i] / 255.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_f32_range_neg1_1"
static inline void cast_u8_to_f32_range_neg1_1(const uint8_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,255] to [-1,1]
        output[i] = ((float)input[i] / 127.5f) - 1.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_i8"
static inline void cast_u8_to_i8(const uint8_t* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,255] to [-128,127]
        output[i] = (int8_t)((int)input[i] - 128);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_i16"
static inline void cast_u8_to_i16(const uint8_t* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int16_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_i32"
static inline void cast_u8_to_i32(const uint8_t* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_u16"
static inline void cast_u8_to_u16(const uint8_t* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (uint16_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u8_to_u32"
static inline void cast_u8_to_u32(const uint8_t* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (uint32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

// UInt16 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_f32_clamp"
static inline void cast_u16_to_f32_clamp(const uint16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_f32_range_0_1"
static inline void cast_u16_to_f32_range_0_1(const uint16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,65535] to [0,1]
        output[i] = (float)input[i] / 65535.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_f32_range_neg1_1"
static inline void cast_u16_to_f32_range_neg1_1(const uint16_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,65535] to [-1,1]
        output[i] = ((float)input[i] / 32767.5f) - 1.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_i8"
static inline void cast_u16_to_i8(const uint16_t* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,65535] to [-128,127]
        // Scale down from 16-bit to 8-bit range, then shift to signed
        uint8_t scaled = (uint8_t)(input[i] >> 8);   // Scale to [0,255]
        output[i] = (int8_t)((int)scaled - 128);     // Shift to [-128,127]
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_i16"
static inline void cast_u16_to_i16(const uint16_t* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,65535] to [-32768,32767]
        output[i] = (int16_t)((int32_t)input[i] - 32768);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_i32"
static inline void cast_u16_to_i32(const uint16_t* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (int32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_u8"
static inline void cast_u16_to_u8(const uint16_t* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,65535] to [0,255]
        // Scale down from 16-bit to 8-bit range
        output[i] = (uint8_t)(input[i] >> 8);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u16_to_u32"
static inline void cast_u16_to_u32(const uint16_t* restrict input, uint32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (uint32_t)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

// UInt32 to other types
#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_f32_clamp"
static inline void cast_u32_to_f32_clamp(const uint32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        output[i] = (float)input[i];
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_f32_range_0_1"
static inline void cast_u32_to_f32_range_0_1(const uint32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [0,1]
        output[i] = (double)input[i] / 4294967295.0;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_f32_range_neg1_1"
static inline void cast_u32_to_f32_range_neg1_1(const uint32_t* restrict input, float* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [-1,1]
        output[i] = ((double)input[i] / 2147483647.5) - 1.0;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_i8"
static inline void cast_u32_to_i8(const uint32_t* restrict input, int8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [-128,127]
        // Scale down from 32-bit to 8-bit range, then shift to signed
        uint8_t scaled = (uint8_t)(input[i] >> 24);  // Scale to [0,255]
        output[i] = (int8_t)((int)scaled - 128);     // Shift to [-128,127]
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_i16"
static inline void cast_u32_to_i16(const uint32_t* restrict input, int16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [-32768,32767]
        // Scale down from 32-bit to 16-bit range, then shift to signed
        uint16_t scaled = (uint16_t)(input[i] >> 16);  // Scale to [0,65535]
        output[i] = (int16_t)((int32_t)scaled - 32768); // Shift to [-32768,32767]
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_i32"
static inline void cast_u32_to_i32(const uint32_t* restrict input, int32_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [-2147483648,2147483647]
        output[i] = (int32_t)((int64_t)input[i] - 2147483648LL);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_u8"
static inline void cast_u32_to_u8(const uint32_t* restrict input, uint8_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [0,255]
        // Scale down from 32-bit to 8-bit range
        output[i] = (uint8_t)(input[i] >> 24);
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cast_u32_to_u16"
static inline void cast_u32_to_u16(const uint32_t* restrict input, uint16_t* restrict output, int32_t count)
{
    for (int i = 0; i < count; i++) {
        // Range conversion: map [0,4294967295] to [0,65535]
        // Scale down from 32-bit to 16-bit range
        output[i] = (uint16_t)(input[i] >> 16);
    }
}
#pragma IMAGINET_FRAGMENT_END 