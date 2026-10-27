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

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"

static inline void object_detection_class_select_f32(const float *restrict input,
                                              const void *restrict index_table,
                                              int step, int size, int slot, int out_size,
                                              float *restrict output)
{
// A row is a single element when the feature axis is innermost, so memcpy per row would be slower.
if (step == 1) {
const int16_t *idx = (const int16_t *)index_table;

for (int j = 0; j < slot; j++) {
const float *ip = input + j * size;
float *op = output + j * out_size;

for (int k = 0; k < out_size; k++)
op[k] = ip[idx[k]];
}
} else {
object_detection_class_select_rows(input, index_table, step, size, slot, out_size, sizeof(float), output);
}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "object_detection_class_select_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "object_detection_class_select_rows"

static inline void object_detection_class_select_i8(const int8_t *restrict input,
                                             const void *restrict index_table,
                                             int step, int size, int slot, int out_size,
                                             int8_t *restrict output)
{
if (step == 1) {
const int16_t *idx = (const int16_t *)index_table;

for (int j = 0; j < slot; j++) {
const int8_t *ip = input + j * size;
int8_t *op = output + j * out_size;

for (int k = 0; k < out_size; k++)
op[k] = ip[idx[k]];
}
} else {
object_detection_class_select_rows(input, index_table, step, size, slot, out_size, sizeof(int8_t), output);
}
}

#pragma IMAGINET_FRAGMENT_END