#pragma once
#include <windows.h>

#define VPX_MD5_DIGEST_SIZE		16
#define VPX_MD5_BLOCK_SIZE		64

#define VPX_MD5_EMPTY			"d41d8cd98f00b204e9800998ecf8427e"

#define VPX_MD5_STRING_SIZE		(VPX_MD5_DIGEST_SIZE*2)
typedef char vpx_md5string_t[VPX_MD5_STRING_SIZE + 1]; /**< Hexadecimal MD5 string. */
typedef UINT8 vpx_md5digest_t[VPX_MD5_DIGEST_SIZE]; /**< MD5 digest bytes. */

#define VPX_MD5_DIGEST_CALC(input, input_size, digest)		\
	{														\
		vpx_md5context_t ctx;								\
		vpx_md5init(&ctx);									\
		vpx_md5update(&ctx, (const UINT8*)(input), (input_size));			\
		vpx_md5final((digest), &ctx);						\
	}

typedef struct vpx_md5context_s
{
	UINT32 buf[4];
	UINT32 bytes[2];
	UINT32 in[16];
}
vpx_md5context_t;

void vpx_md5init(vpx_md5context_t *context);
void vpx_md5update(vpx_md5context_t *context, UINT8 const *buf, size_t len);
void vpx_md5final(vpx_md5digest_t digest, vpx_md5context_t *context);
void vpx_md5transform(UINT32 buf[4], UINT32 const in[VPX_MD5_DIGEST_SIZE]);
int vpx_md5compute(const char* input, size_t size, vpx_md5string_t *result);
