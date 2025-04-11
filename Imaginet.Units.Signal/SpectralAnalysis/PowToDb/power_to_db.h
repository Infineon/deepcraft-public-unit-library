#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <float.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "power_to_db_f32"
static inline void power_to_db_f32(const float* restrict x, int count, float beta, float amin, float topdb, float* restrict result)
{
	float gamma = -FLT_MAX;
	for(int i = 0; i < count; i++)
	{
		float value = x[i];
		if (amin > value)
			value = amin;
		value = 10 * log10f(value) - beta;
		result[i] = value;
		if (value > gamma)
			gamma = value;
	}

	gamma -= topdb;
	for (int i = 0; i < count; i++)
	{
		float value = result[i];
		if (gamma > value)
			result[i] = gamma;
	}
}
#pragma IMAGINET_FRAGMENT_END
