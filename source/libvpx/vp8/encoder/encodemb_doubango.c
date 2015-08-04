#include "vpx_config.h"

#if CONFIG_DOUBANGO

#include "doubango_config.h"

#include <mmintrin.h> /* MMX */
#include <xmmintrin.h> /* SSE */
#include <emmintrin.h> /* SSE2 */

// Less instructions and cycles than google's code (useless sign saving)
// No loop and jumps
void vp8_subtract_mby_doubango_sse2(short *diff, unsigned char *src, int src_stride, unsigned char *pred, int pred_stride)
{
#if DOUBANGO_ASM_INLINE && !defined(_M_X64)
#define ASM_DIFF() \
	__asm movdqa xmm4, xmmword ptr[eax] \
	__asm movdqa xmm5, xmmword ptr[ecx] \
		 \
	__asm movdqa xmm1, xmm4 \
	__asm movdqa xmm2, xmm5 \
	__asm punpcklbw xmm1, xmm0 \
	__asm punpcklbw xmm2, xmm0 \
	__asm psubsw xmm1, xmm2 \
	__asm movdqa xmmword ptr[edx], xmm1 \
	__asm add edx, 10h \
		 \
	__asm punpckhbw xmm4, xmm0 \
	__asm punpckhbw xmm5, xmm0 \
	__asm psubsw xmm4, xmm5 \
	__asm movdqa xmmword ptr[edx], xmm4 \
	__asm add edx, 10h

#define ASM_DIFF_AND_GOTO_NEXT() \
	ASM_DIFF() \
	__asm add eax, dword ptr[src_stride] \
	__asm add ecx, dword ptr[pred_stride]

	__asm pxor xmm0, xmm0
	__asm mov edx, dword ptr[diff]
	__asm mov eax, dword ptr[src]
	__asm mov ecx, dword ptr[pred]

	ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT()
	ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT()
	ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT()
	ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF_AND_GOTO_NEXT() ASM_DIFF()

#undef ASM_DIFF
#undef ASM_DIFF_AND_GOTO_NEXT
#else
	int r;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm5;

	_mm_store_si128(&xmm0, _mm_setzero_si128());

	for (r = 0; r < 16; r++)
	{
		xmm4 = _mm_load_si128((__m128i*)src);
		xmm5 = _mm_load_si128((__m128i*)pred);

		xmm1 = _mm_unpacklo_epi8(xmm4, xmm0);
		xmm2 = _mm_unpacklo_epi8(xmm5, xmm0);
		xmm2 = _mm_subs_epi16(xmm1, xmm2);
		_mm_store_si128((__m128i*)diff, xmm2);

		xmm1 = _mm_unpackhi_epi8(xmm4, xmm0);
		xmm2 = _mm_unpackhi_epi8(xmm5, xmm0);
		xmm2 = _mm_subs_epi16(xmm1, xmm2);
		_mm_store_si128((__m128i*)(diff + 8), xmm2);

		diff += 16;
		pred += pred_stride;
		src += src_stride;
	}
#endif
}

#endif /* CONFIG_DOUBANGO */
