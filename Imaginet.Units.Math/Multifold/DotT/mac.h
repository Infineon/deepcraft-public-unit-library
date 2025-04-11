/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* File created: 2020-05-27 Albert Seward (albert.seward@imagimob.com)
*/

#pragma IMAGINET_FRAGMENT_BEGIN "dot_mac_f32"

static inline float dot_mac_f32(const float* restrict a, const float* restrict b, int count)
{
	float sum = 0;
	for (int i = 0; i < count; i++) {
		sum += *a++ * *b++;
	}
	return sum;
}

#pragma IMAGINET_FRAGMENT_END
