#include "vpx_config.h"

#if CONFIG_DOUBANGO

#include "doubango_config.h"

#include <mmintrin.h> /* MMX */
#include <xmmintrin.h> /* SSE */
#include <emmintrin.h> /* SSE2 */

// SSE3 version from Goggle doesn't check for "max_sad" to break and always consider that "ref_ptr" is unaligned
// TODO: add ASM version
unsigned int vp8_sad16x16_doubango_sse2(const unsigned char *src_ptr/*always aligned*/, int src_stride/*always aligned*/,
	const unsigned char *ref_ptr, int ref_stride,
	unsigned int max_sad)
{	
	unsigned int sad = 0;
	__m128i xmm0;

	if (DOUBANGO_IS_ALIGNED(ref_ptr, 16) && DOUBANGO_IS_ALIGNED(ref_stride, 16)) {
#define MOVE_ALIGNED() _mm_store_si128(&xmm0, _mm_sad_epu8(_mm_load_si128((__m128i*)src_ptr), _mm_load_si128((__m128i*)ref_ptr))); \
		if ((sad += xmm0.m128i_i16[0] + xmm0.m128i_i16[4]) > max_sad) return sad
#define MOVE_ALIGNED_AND_GOTO_NEXT() MOVE_ALIGNED(); src_ptr += src_stride; ref_ptr += ref_stride

		MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT();
		MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT();
		MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT();
		MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED_AND_GOTO_NEXT(); MOVE_ALIGNED();
#undef MOVE_ALIGNED
#undef MOVE_ALIGNED_AND_GOTO_NEXT
	}
	else {
#define MOVE_UNALIGNED() _mm_store_si128(&xmm0, _mm_sad_epu8(_mm_load_si128((__m128i*)src_ptr), _mm_loadu_si128((__m128i*)ref_ptr))); \
		if ((sad += xmm0.m128i_i16[0] + xmm0.m128i_i16[4]) > max_sad) return sad
#define MOVE_UNALIGNED_AND_GOTO_NEXT() MOVE_UNALIGNED(); src_ptr += src_stride; ref_ptr += ref_stride

		MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT();
		MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT();
		MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT();
		MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED_AND_GOTO_NEXT(); MOVE_UNALIGNED();
#undef MOVE_UNALIGNED
#undef MOVE_UNALIGNED_AND_GOTO_NEXT
	}

	return sad;
}

// SSE3 version from Goggle doesn't check for alignment
// TODO: add ASM version
void vp8_copy32xn_doubango_sse2(const unsigned char *src_ptr, int src_stride,
	const unsigned char *dst_ptr, int dst_stride,
	int height)
{
	register int r;
	if (DOUBANGO_IS_ALIGNED(dst_ptr, 16) && DOUBANGO_IS_ALIGNED(dst_stride, 16)) { // "dst_ptr"/"dst_stride" -> higher prob to be aligned
		if (DOUBANGO_IS_ALIGNED(src_ptr, 16) && DOUBANGO_IS_ALIGNED(src_stride, 16)) {
			for (r = 0; r < height; r++) {
				_mm_store_si128((__m128i*)dst_ptr, _mm_load_si128((__m128i*)src_ptr)); _mm_store_si128((__m128i*)&dst_ptr[16], _mm_load_si128((__m128i*)&src_ptr[16])); src_ptr += src_stride; dst_ptr += dst_stride;
			}
		}
		else {
			for (r = 0; r < height; r++) {
				_mm_store_si128((__m128i*)dst_ptr, _mm_loadu_si128((__m128i*)src_ptr)); _mm_store_si128((__m128i*)&dst_ptr[16], _mm_loadu_si128((__m128i*)&src_ptr[16])); src_ptr += src_stride; dst_ptr += dst_stride;
			}
		}
	}
	else {
		for (r = 0; r < height; r++) {
			_mm_storeu_si128((__m128i*)dst_ptr, _mm_loadu_si128((__m128i*)src_ptr)); _mm_storeu_si128((__m128i*)&dst_ptr[16], _mm_loadu_si128((__m128i*)&src_ptr[16])); src_ptr += src_stride; dst_ptr += dst_stride;
		}
	}
}

#endif /* CONFIG_DOUBANGO */
