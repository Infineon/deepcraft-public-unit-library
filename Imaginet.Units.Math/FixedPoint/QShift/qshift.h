#pragma IMAGINET_FRAGMENT_BEGIN "qshift_q31"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void qshift_q31(const q31_t* restrict src, int shift, q31_t* restrict dst, int count)
{
	if (shift < 0){
		for(int i = 0; i < count; i++) {
			*dst++ = (q31_t)__SAT((q63_t)*src++ << (-shift), 32);
		}
	} else {
		for (int i = 0; i < count; i++) {
			*dst++ = (q31_t)__SAT((q63_t)*src++ >> shift, 32);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "qshift_q15"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void qshift_q15(const q15_t* restrict src, int shift, q15_t* restrict dst, int count)
{
	if (shift < 0) {
		for (int i = 0; i < count; i++) {
			*dst++ = (q15_t)__SSAT(((q31_t)*src++) << (-shift), 16);
		}
	} else {
		for (int i = 0; i < count; i++) {
			*dst++ = (q15_t)__SSAT(((q31_t)*src++) >> shift, 16);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "qshift_q7"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void qshift_q7(const q7_t* restrict src, int shift, q7_t* restrict dst, int count)
{
	if (shift < 0){
		for (int i = 0; i < count; i++) {
			*dst++ = (q7_t)__SSAT(((q15_t)*src++) << (-shift), 8);
		}
	} else {
		for (int i = 0; i < count; i++) {
			*dst++ = (q7_t)__SSAT(((q15_t)*src++) >> shift, 8);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END