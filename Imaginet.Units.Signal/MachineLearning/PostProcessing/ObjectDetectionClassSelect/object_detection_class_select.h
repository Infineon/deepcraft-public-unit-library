#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_init"

#define OBJECT_DETECTION_CLASS_SELECT_MAX 16

// Builds the row lookup table used by the gather functions.
//
// The feature axis of a detection head is laid out as
// [box values][class scores]. Rows 0...box_count-1 are the box values and are
// always kept; r0...r15 are the class rows to keep after them, in order.
//
// The .imunit contracts already reject out-of-range rows at graph build time;
// the checks here are a backstop for hand-edited graphs.
static inline int object_detection_class_select_init(void *restrict index_table,
                                              int box_count, int feature_count, int selected_count,
                                              int r0, int r1, int r2, int r3,
                                              int r4, int r5, int r6, int r7,
                                              int r8, int r9, int r10, int r11,
                                              int r12, int r13, int r14, int r15)
{
	const int selected[OBJECT_DETECTION_CLASS_SELECT_MAX] = {
		r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15
	};
	int16_t *table = (int16_t *)index_table;

	if (box_count < 0 || feature_count < 1 || box_count >= feature_count)
		return -2;

	if (selected_count < 0 || selected_count > OBJECT_DETECTION_CLASS_SELECT_MAX)
		return -2;

	for (int i = 0; i < box_count; i++)
		table[i] = (int16_t)i;

	for (int k = 0; k < selected_count; k++) {
		const int r = selected[k];

		if (r < box_count || r >= feature_count)
			return -2;

		table[box_count + k] = (int16_t)r;
	}

	return 0;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_rows"

// Copies out_size rows of step elements each, picked by the lookup table.
//
// step      = input.shape.step(axis)
// size      = input.shape.size(axis)
// slot      = input.shape.slot(axis)
// elem_size = bytes per element
static inline void object_detection_class_select_rows(const void *restrict input,
                                               const void *restrict index_table,
                                               int step, int size, int slot, int out_size,
                                               int elem_size, void *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const int row_bytes = step * elem_size;
	const int in_stride = row_bytes * size;
	const int out_stride = row_bytes * out_size;

	const uint8_t *in = (const uint8_t *)input;
	uint8_t *out = (uint8_t *)output;

	for (int j = 0; j < slot; j++) {
		for (int k = 0; k < out_size; k++)
			memcpy(out + k * row_bytes, in + idx[k] * row_bytes, (size_t)row_bytes);

		in += in_stride;
		out += out_stride;
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_lanes"

// Same gather as object_detection_class_select_rows, but for layouts where the
// feature axis is innermost (step == 1). A row is then a single element, so
// memcpy would degenerate into one call per element.
static inline void object_detection_class_select_lanes(const void *restrict input,
                                                const void *restrict index_table,
                                                int size, int slot, int out_size,
                                                int elem_size, void *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const uint8_t *in = (const uint8_t *)input;
	uint8_t *out = (uint8_t *)output;

	switch (elem_size) {
	case 1:
		for (int j = 0; j < slot; j++) {
			const int8_t *ip = (const int8_t *)in + j * size;
			int8_t *op = (int8_t *)out + j * out_size;
			for (int k = 0; k < out_size; k++)
				op[k] = ip[idx[k]];
		}
		break;
	case 2:
		for (int j = 0; j < slot; j++) {
			const int16_t *ip = (const int16_t *)in + j * size;
			int16_t *op = (int16_t *)out + j * out_size;
			for (int k = 0; k < out_size; k++)
				op[k] = ip[idx[k]];
		}
		break;
	default:
		for (int j = 0; j < slot; j++) {
			const int32_t *ip = (const int32_t *)in + j * size;
			int32_t *op = (int32_t *)out + j * out_size;
			for (int k = 0; k < out_size; k++)
				op[k] = ip[idx[k]];
		}
		break;
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_f32(const float *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              float *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 4, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 4, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_i8(const int8_t *restrict input,
                                             const void *restrict index_table,
                                             int step, int size, int slot, int out_size,
                                             int8_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 1, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 1, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i16"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_i16(const int16_t *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              int16_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 2, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 2, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_i32(const int32_t *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              int32_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 4, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 4, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_u8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_u8(const uint8_t *restrict input,
                                             const void *restrict index_table,
                                             int step, int size, int slot, int out_size,
                                             uint8_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 1, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 1, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_u16"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_u16(const uint16_t *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              uint16_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 2, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 2, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_u32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_lanes"

static inline void object_detection_class_select_u32(const uint32_t *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              uint32_t *restrict output)
{
	if (step == 1)
		object_detection_class_select_lanes(input, index_table, size, slot, out_size, 4, output);
	else
		object_detection_class_select_rows(input, index_table, step, size, slot, out_size, 4, output);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i8_to_f32"

// Gather and convert in one pass. Range mapping matches the Cast unit's
// "Range map [0,1]" mode, which is what the rest of the detection chain uses.
static inline void object_detection_class_select_i8_to_f32(const int8_t *restrict input,
                                                    const void *restrict index_table,
                                                    int step, int size, int slot, int out_size,
                                                    float *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const int in_stride = step * size;
	const int out_stride = step * out_size;

	for (int j = 0; j < slot; j++) {
		const int8_t *ip = input + j * in_stride;
		float *op = output + j * out_stride;

		for (int k = 0; k < out_size; k++) {
			const int8_t *src = ip + idx[k] * step;
			float *dst = op + k * step;

			for (int i = 0; i < step; i++)
				dst[i] = ((float)src[i] + 128.0f) * (1.0f / 255.0f);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i16_to_f32"

static inline void object_detection_class_select_i16_to_f32(const int16_t *restrict input,
                                                     const void *restrict index_table,
                                                     int step, int size, int slot, int out_size,
                                                     float *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const int in_stride = step * size;
	const int out_stride = step * out_size;

	for (int j = 0; j < slot; j++) {
		const int16_t *ip = input + j * in_stride;
		float *op = output + j * out_stride;

		for (int k = 0; k < out_size; k++) {
			const int16_t *src = ip + idx[k] * step;
			float *dst = op + k * step;

			for (int i = 0; i < step; i++)
				dst[i] = ((float)src[i] + 32768.0f) * (1.0f / 65535.0f);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_u8_to_f32"

static inline void object_detection_class_select_u8_to_f32(const uint8_t *restrict input,
                                                    const void *restrict index_table,
                                                    int step, int size, int slot, int out_size,
                                                    float *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const int in_stride = step * size;
	const int out_stride = step * out_size;

	for (int j = 0; j < slot; j++) {
		const uint8_t *ip = input + j * in_stride;
		float *op = output + j * out_stride;

		for (int k = 0; k < out_size; k++) {
			const uint8_t *src = ip + idx[k] * step;
			float *dst = op + k * step;

			for (int i = 0; i < step; i++)
				dst[i] = (float)src[i] * (1.0f / 255.0f);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_f32_to_i8"

static inline void object_detection_class_select_f32_to_i8(const float *restrict input,
                                                    const void *restrict index_table,
                                                    int step, int size, int slot, int out_size,
                                                    int8_t *restrict output)
{
	const int16_t *idx = (const int16_t *)index_table;
	const int in_stride = step * size;
	const int out_stride = step * out_size;

	for (int j = 0; j < slot; j++) {
		const float *ip = input + j * in_stride;
		int8_t *op = output + j * out_stride;

		for (int k = 0; k < out_size; k++) {
			const float *src = ip + idx[k] * step;
			int8_t *dst = op + k * step;

			for (int i = 0; i < step; i++) {
				float v = src[i];

				if (v < 0.0f)
					v = 0.0f;
				else if (v > 1.0f)
					v = 1.0f;

				// Rounded before the bias so truncation stays correct at v == 0.
				dst[i] = (int8_t)((int)(v * 255.0f + 0.5f) - 128);
			}
		}
	}
}

#pragma IMAGINET_FRAGMENT_END
