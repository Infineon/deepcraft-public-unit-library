#pragma IMAGINET_FRAGMENT_BEGIN "bit_utilization_q31"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void bit_utilization_q31(const q31_t* restrict src, int8_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = 32 - __builtin_clrsb(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "bit_utilization_q15"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void bit_utilization_q15(const q15_t* restrict src, int8_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = 32 - __builtin_clrsb(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "bit_utilization_q7"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void bit_utilization_q7(const q7_t* restrict src, int8_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = 32 - __builtin_clrsb(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END