// test_fast.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include "stdafx.h"
#include "md5.h"
#define VPX_CODEC_DISABLE_COMPAT 1 /* strict compliance with the latest SDK by disabling some backwards compatibility  */
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

#define VPX_MD5_COMPARE			0
#define VPX_MD5_PRINT			0

#define vp8_interface_enc (vpx_codec_vp8_cx())
#define vp8_interface_dec (vpx_codec_vp8_dx())

#define VPX_DEBUG_INFO(FMT, ...) fprintf(stderr, "*[LIBVPX INFO]: " FMT "\n", ##__VA_ARGS__)
#define VPX_DEBUG_WARN(FMT, ...) fprintf(stderr, "**[LIBVPX WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#define VPX_DEBUG_ERROR(FMT, ...) fprintf(stderr, "***[LIBVPX ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#define VPX_DEBUG_FATAL(FMT, ...) fprintf(stderr, "****[LIBVPX FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)

#define VPX_NUM_CORES	2

#define VPX_MOTION_RANK	2

#define VPX_FPS			100
#define VPX_GOP_IN_SEC	25

#define VPX_INPUT_WIDTH			1024
#define VPX_INPUT_HEIGHT		768
#define VPX_INPUT_SIZE			((VPX_INPUT_WIDTH * VPX_INPUT_HEIGHT * 3) >> 1)

#define VPX_LOOP_COUNT			100

static const char* MD5_Values[] = {
	"d2c7c0a3f721a88095f77816f0bdaf2a", "34e604f964d5ab76897b6126aa111882", "e6f63c3fd0358ae9fb56024e8023cd28","5ad03dc700549c7a002630e82c6b7f4f","a94daeb907690d5f161c31288b8d4c5b",
	"34162d58a468abf561b6c866ea31705b","fdd88bcffb13e03bc0fa03bf1bd3d530","1d06e50b3d2c0ab273e2d8c948c5a926","4b9ad4635d54baa8ad5b21ccaeeec5be","b39909860de1b1d72dffd5bf8a9d127c",
	"2fe785260126bab3ee8d2fb609403287","877a17ce906b6007e27e39702721a85a","723757a3f4da3d9be3affdf6837121d7","32bac0489fcd128c19017539866f65ed","2d5e8d6225c9c5dbe960ef4d2bef8957",
	"04cab1517f951a5f38bdfa9edb6b662c","b4ce18948ed79e5bb199ef5cc29bb475", "fb65fd974143938833313b7dc4f07f75", "eb23a4630b2f31146a35f435dab6f62c","afe9f20af3d5b3ba19aeb3b6699e6acb",
	"180a43be314afe46d88a0e3af3e1c82f","e0898ee6e65761ac901d548f03783ec1","f7925abff45630584940e684ffd30a3e","41f37852e53ea7ca70ac42aea04545a0","47724de6bd46df83e15e6221d6f5359a",
	"72b39bfa7c5a135307aef58fb7feae80","ae7ebc1618761c6c04ea8071592e63c8","d32c4644304e3e5dc9b57e07ecb4440d","f34fa4381b974bc379fe94ba36ce21e0","b5cfdc2ce0b66fb43f8a97a8365a6678",
	"bfb205c673dbbcfec76afecbca0645b3","94b6be9ab31be941a49bd2b9a74a45f3","cbbc4002524de9e5e1bf2734d56a8b25", "13033e895c3381b04d93a074c05c0d26", "00a2d1e5ec72b6e0e8dcbab5a5e3854f",
	"0c01fb4829768e8138119482810cec87","36abda59db000dc46a6c123564af2bff","562773af37b76c738e8064caf7a6c5f8","d1be9222e7c6327aa5ce6cb61bd79f35","07f1c7053607a281de243c346c1ff7ab",
	"71da3defd4cad4832baae6cf6a8b0103","3167237c3c98b5d989ce343d8b214e89","2111edc94146c30058d1e82a0c09f3ce","3e78e672107c51c86f7e3b4b3f280c09","3324f4bad18df33706efc4c06b41afb5",
	"9c2e04c4cdca0e7f294f2a8f6944121f","8a13920c62c9eadb33d056a620ce9c4e","b4c5f6c8898a597db8a7b562549d086f","5927e794452f6559006919c987fc5bc8","d7c2a1d2a1629717a90538d6582781d8",
	"a5a27fe2a97617c7ac64f8dc870d93b5","300df6173ca249137ae2d8017c7e6ab5","9485078af2d20e0ff18140de33dd8caa","3beb907121e247d2e828222ff906cc45","ff59ea0c1b98f3dccfade6d25e95fd05",
	"2f403cfad859074de6ac2dd990662af2","f975aa4ef6c97fd5ecafba08bdfbea48","9d9a241b6812e66b050083cc878099c5","59484be1d5c5634d3a6bf17bf8c8da21","e2f24670bd5e31f4236a82500af50cef",
	"70c5404dfbd3e1d5d91927fe1a7a66f8","af3fcf64ab859a52483753f1e2e3f94e","fd1bb99dc9b6672cc9378eb2e054d173","e10b8e9650a5cd6e77e253a58044c10c","bbc2b1ec86ccd0f97aa16631189465e1",
	"756bb193549e978a7af13759de226f3f","cf56e9614c385b2fd12360fb411122ef","f648a4f4cdaef40aeb2d7938764174cd","061831975bc0ba2bc83ed9c9456b9c87","b676fe940145af7ffaf8f74b6b103d5a",
	"2b447ad15f9fb2086cc5e6a23e3f223b","a07fdd3416da006a3e4fedcb7582eedd","bcbd63131234fa1a8a40006812389501","4415343964164bc1c2e706b4407f00bd","01765162bc1bb188c81001c5649f202f",
	"7106b7a7567331f5e7aa37fd06f11dfd","b48b86773d5b83b91967e1181ac6e36e","165419da5e794fae0c33013fe396a585","db32be2b79194d531c40bc186bd4b2d3","5911bc89b3db9477f0c9b586f5696573",
	"f142fc6e9d21c2fd744c9589b34e4959","20fadc4f17d5aeea7ebe6a90a96b3723","1289f66ba30136f83ad55762598047fa","af545e6a0bcc0f94e8cabf9b9b574f0b","6789047b355074c3cc0c35817c2e1d3c",
	"32b9f9ed42ddbc5b0c6bf5530f2aed36","3355bce2db203e4df1a9c3a9d2bbd11c","c8b1c3d67296cacaf84076c1d20d382b","e940cee507cd149ab331aca05568750c","7cb4f264493a9cb48f64dc45f437276b",
	"a9ff026f7b0236d87f6b0ab3c55e24ca","4cf74a4baa82df9756d76fab7753bec0","13a7b509970555a20c658e581117a273","5279f84ee248fd593d88918081949a7a","dea7a3469f0713cbdcbd776ab0d435d3",
	"e7b8af3c8cb84a0717d0a14a792e618e","5b014cdd5eff25d0baa3cae5cf05e367", "c1b1271b4027258db77a859d94b4fb13", "a08c3d3370bd353cb502d7d8c445acaa","ca393aafdec28d9613f3304f2176d04f"
};

static const size_t MD5_ValuesCount = sizeof(MD5_Values) / sizeof(MD5_Values[0]);

static void VpxFillSamples(unsigned char *ptr, size_t size, int index)
{
	for (size_t i = 0; i < size; ++i)
	{
		ptr[i] = i % (0xFF - index);
	}
}

static BOOL VpxBuffEquals(const void* buff1Ptr, size_t buff1Size, const void* buff2Ptr, size_t buff2Size)
{
	if (!buff1Ptr || !buff2Ptr || buff1Size != buff2Size) {
		return FALSE;
	}
	const UINT8 *_buff1Ptr = (const UINT8 *)buff1Ptr;
	const UINT8 *_buff2Ptr = (const UINT8 *)buff2Ptr;
	for (size_t i = 0; i < buff1Size; ++i)
	{
		if (_buff1Ptr[i] != _buff2Ptr[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}
#define VpxMd5BuffEquals(buff1Ptr, buff2Ptr) VpxBuffEquals((buff1Ptr), VPX_MD5_STRING_SIZE, (buff2Ptr), VPX_MD5_STRING_SIZE)

static UINT64 VpxTimeNow()
{
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
	static int __cpu_count = 0;
	if (__cpu_count == 0){
		SYSTEM_INFO SystemInfo;
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
		GetNativeSystemInfo(&SystemInfo);
#	else
		GetSystemInfo(&SystemInfo);
#	endif
		__cpu_count = SystemInfo.dwNumberOfProcessors;
	}
	if (__cpu_count == 1){
		static LARGE_INTEGER __liFrequency = { 0 };
		LARGE_INTEGER liPerformanceCount;
		if (!__liFrequency.QuadPart){
			QueryPerformanceFrequency(&__liFrequency);
		}
		QueryPerformanceCounter(&liPerformanceCount);
		return (UINT64)(((double)liPerformanceCount.QuadPart / (double)__liFrequency.QuadPart)*1000.0);
	}
	else{
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
		return tsk_time_epoch();
#	else
		return timeGetTime();
#	endif
	}
#elif HAVE_CLOCK_GETTIME || _POSIX_TIMERS > 0
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (((uint64)ts.tv_sec)*(uint64)1000) + (((uint64)ts.tv_nsec) / (uint64)1000000);
#elif defined(__APPLE__)
	static mach_timebase_info_data_t __apple_timebase_info = { 0, 0 };
	if (__apple_timebase_info.denom == 0) {
		(void)mach_timebase_info(&__apple_timebase_info);
	}
	return (uint64)((mach_absolute_time() * __apple_timebase_info.numer) / (1e+6 * __apple_timebase_info.denom));
#else
	struct timeval tv;
	gettimeofday(&tv, tsk_null);
	return (((uint64)tv.tv_sec)*(uint64)1000) + (((uint64)tv.tv_usec) / (uint64)1000);
#endif
}

int _tmain(int argc, _TCHAR* argv[])
{
	vpx_codec_enc_cfg_t cfg = {0};
	vpx_codec_ctx_t context = {0};
	vpx_codec_err_t vpx_ret;
	vpx_enc_frame_flags_t enc_flags = 0; // VPX_EFLAG_XXX
	vpx_image_t image;
	unsigned char *inPtr = NULL, *outPtr = NULL;
	vpx_codec_pts_t pts = 1;
	UINT64 timeStart, timeEnd;

	VPX_DEBUG_INFO("**** Start (VPX_MD5_COMPARE=%d VPX_MD5_PRINT=%d) ****", VPX_MD5_COMPARE, VPX_MD5_PRINT);

	inPtr = (unsigned char *)malloc(VPX_INPUT_SIZE); 
	if (!inPtr) {
		VPX_DEBUG_ERROR("Failed to allocate input");
		goto bail;
	}
	outPtr = (unsigned char *)malloc(VPX_INPUT_SIZE); 
	if (!outPtr) {
		VPX_DEBUG_ERROR("Failed to allocate output");
		goto bail;
	}

	if ((vpx_ret = vpx_codec_enc_config_default(vp8_interface_enc, &cfg, 0)) != VPX_CODEC_OK) {
		VPX_DEBUG_ERROR("vpx_codec_enc_config_default failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		goto bail;
	}

	cfg.g_timebase.num = 1;
	cfg.g_timebase.den = VPX_FPS;
	cfg.g_w = VPX_INPUT_WIDTH;
	cfg.g_h = VPX_INPUT_HEIGHT;
	cfg.rc_target_bitrate = (int)((VPX_INPUT_WIDTH * VPX_INPUT_HEIGHT * VPX_FPS * VPX_MOTION_RANK * 0.07) / 1024);
	cfg.kf_mode = VPX_KF_DISABLED;
	cfg.kf_min_dist = cfg.kf_max_dist = (VPX_GOP_IN_SEC * VPX_FPS);
	cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
	cfg.g_lag_in_frames = 0;
	cfg.g_threads = VPX_NUM_CORES;
	cfg.rc_end_usage = VPX_CBR;
	cfg.g_pass = VPX_RC_ONE_PASS;

	if ((vpx_ret = vpx_codec_enc_init(&context, vp8_interface_enc, &cfg, enc_flags)) != VPX_CODEC_OK) {
		VPX_DEBUG_ERROR("vpx_codec_enc_init failed with error =%s", vpx_codec_err_to_string(vpx_ret));
		goto bail;
	}

	{
		vpx_enc_frame_flags_t flags = 0;
		const vpx_codec_cx_pkt_t *pkt = NULL;
		vpx_codec_iter_t iter = NULL;
		timeStart = VpxTimeNow();
		for (int index = 0; index < VPX_LOOP_COUNT; ++index) {
			VpxFillSamples(inPtr, VPX_INPUT_SIZE, index);
			if (!vpx_img_wrap(&image, VPX_IMG_FMT_I420, VPX_INPUT_WIDTH, VPX_INPUT_HEIGHT, 1, inPtr)) {
				VPX_DEBUG_ERROR("vpx_img_wrap failed");
				goto bail;
			}
			if ((vpx_ret = vpx_codec_encode(&context, &image, pts++, 1, flags, VPX_DL_REALTIME)) != VPX_CODEC_OK) {
				VPX_DEBUG_ERROR("vpx_codec_encode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
				goto bail;
			}
			vpx_img_free(&image);
#if VPX_MD5_COMPARE || VPX_MD5_PRINT
			iter = NULL;
			while ((pkt = vpx_codec_get_cx_data(&context, &iter))) {
				switch (pkt->kind) {
				case VPX_CODEC_CX_FRAME_PKT:
					{
						vpx_md5string_t md5;
						if (vpx_md5compute((const char*)pkt->data.frame.buf, pkt->data.frame.sz, &md5) != 0) {
							VPX_DEBUG_ERROR("vpx_md5compute failed");
							goto bail;
						}
#if VPX_MD5_COMPARE
						if (index < MD5_ValuesCount) {
							if (VpxMd5BuffEquals(md5, MD5_Values[index]) != TRUE) {
								VPX_DEBUG_ERROR("Md5BuffEquals(%d) failed", index);
								goto bail;
							}
						}
						else {
							VPX_DEBUG_ERROR("i(%d) >= MD5_ValuesCount(%u)", index, MD5_ValuesCount);
							goto bail;
						}
						
#endif
#if VPX_MD5_PRINT
						printf("\"%s\",", md5);
#endif
						break;
					}
				default:
				case VPX_CODEC_STATS_PKT:       /**< Two-pass statistics for this frame */
				case VPX_CODEC_PSNR_PKT:        /**< PSNR statistics for this frame */
				case VPX_CODEC_CUSTOM_PKT: /**< Algorithm extensions  */
					{
						VPX_DEBUG_INFO("pkt->kind=%d not supported", (int)pkt->kind);
						break;
					}
				}
			}
#endif /* #if VPX_MD5_COMPARE || VPX_MD5_PRINT */
		} // for (int i = 0
		timeEnd = VpxTimeNow();
		VPX_DEBUG_INFO("Encoded %d frames in %llu millis\n --> Time for each frame=%.2f millis\n --> Speed=%.2f frames/sec",
			VPX_LOOP_COUNT,
			(timeEnd - timeStart),
			((float)timeEnd - (float)timeStart) / (float)VPX_LOOP_COUNT,
			((float)VPX_LOOP_COUNT / (((float)timeEnd - (float)timeStart) / (float)1000)));
	}

bail:
	vpx_codec_destroy(&context);

	if (inPtr) free(inPtr);
	if (outPtr) free(outPtr);

	VPX_DEBUG_INFO("**** Done!!!! ****");

	getchar();

	return 0;
}

