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

#define VPX_MD5_COMPARE		1
#define VPX_MD5_PRINT		0

#define vp8_interface_enc (vpx_codec_vp8_cx())
#define vp8_interface_dec (vpx_codec_vp8_dx())

#define VPX_DEBUG_INFO(FMT, ...) fprintf(stderr, "*[LIBYUV INFO]: " FMT "\n", ##__VA_ARGS__)
#define VPX_DEBUG_WARN(FMT, ...) fprintf(stderr, "**[LIBYUV WARN]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#define VPX_DEBUG_ERROR(FMT, ...) fprintf(stderr, "***[LIBYUV ERROR]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#define VPX_DEBUG_FATAL(FMT, ...) fprintf(stderr, "****[LIBYUV FATAL]: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)

#define VPX_NUM_CORES	2

#define VPX_MOTION_RANK	2

#define VPX_FPS			15
#define VPX_GOP_IN_SEC	25

#define VPX_INPUT_WIDTH			1024
#define VPX_INPUT_HEIGHT		768
#define VPX_INPUT_SIZE			((VPX_INPUT_WIDTH * VPX_INPUT_HEIGHT * 3) >> 1)

#define VPX_LOOP_COUNT			100

static const char* MD5_Values[] = {
	"2f40dfa1566cfaa37be806eba9438079", "424c788336288dda1cb91f586cfec5ba", "5df070b9149d60e7ac24480fc73ad377", "66002fdbecf83a43583091057189edf3", "1f6b025ff9b2b003ca662d7ef40deeb4",
	"c374af836bb46465a625681aa3fc9672", "e3dc35961a298b97391bb437e912e4b9", "657a96160ac64d2fd10b678440ee11a1", "bbbfb0461b30232ee385cf0a5beffac4", "e3cc4ab96cbbd8da8fc9eeff88b15346",
	"8a4aeff2168096e6b70be56d0fb0db6a", "7a18596146a0e80ad25075deab977b0f", "6fb214dc20b15a02ea5c085cf93104b7", "dc63d75fb8af113b13e38d7786eff941", "6f87d839b6927d051b569148b058cc60",
	"5d4f8001b3b39f73a0c36e7b9c20b640", "8a6c6b05da05d509d4eef00bb98765c8", "3ca9bfb00b39e35fa719743b25234748", "14a7817ff9d86de81044adaf1a9a43e6", "6037de2fff7fb7a7b1c4c1409250d2dc",
	"54922337ed12068201b5e64e8e25e343", "a76379a15612909c6cc89e03c3568c38", "0baba8d8ae9ad56de39afa7deb6b93e0", "217799a2e135ae33cc0e0e8218bd96f8", "21dad382cae2bed9736ed7c53b549a15",
	"5930045c8c5935fdedef3cc7bea0b484", "12c6af4d40694d954458ce65e9653841", "38570b824b5b9752c9453376f9cd16e4", "656d79d8c9e01ef6892eeba3ce63a633", "929940b6a5c1e39a027161e918d79ad4",
	"3ff1174e642d29459670c4a08a5b92a9", "a4a79f5da531839f09dec4103e339223", "09376492245c306662797ccd4f2a9155", "4da94624b2fc8bd2b471fef8a16f6c0a", "df88fdc7b0f798de04d0a8a16c8983c8",
	"5f0288dfb91b4501c7c6dddcba066ae3", "53928eef3f88cd8b7981d92605447052", "67f9df8bbdfd982b5c1135499114309a", "61a4f03b72c2475c034f83b4bfff6ebf", "e57ef1c37750fd494e4306e1006a27fd",
	"f9e107ac45ac461688ecfd15e37bac1a", "35e1e70b5870911dedfebd83dbee2c0c", "7d6d408626013404b1e1473aa8586820", "2979592ce3769b9f9aa55312ecc7f18b", "52499eb23857579047ccd26a40d912d1",
	"c60ac07cf4cdaa8832c7a05e1b17b3e5", "91e3edc1065623c9c9c8e3a63e3dccc2", "5f906b13cb44c732345b29034d7cbc86", "00e36799ab0efe77614f0ba0ed703940", "284276ba3aeb29f1c9fae2b180713c03",
	"68ce143f6e52fe7afc5cae37cb7ba06b", "50daa33ac39b0852dedd37d5fcdb06be", "72235c926db2ed02eb3550bd18a773b6", "7d72ef676b271216ae9a5ae9d18cdd3e", "8a4264032c0e7dc5a8a7095db2206f4e",
	"51b2b0d9b7edbcefbf34ae6621272cae", "75843d0b8bfb9f13fe37e1d33f18bef5", "3b4be6c89b59641fb6942c83326826a1", "57ae21b88cfb38d741346021f3193189", "f0b1b8f3b3aead048079401e7d4a22d0",
	"f8e147c179eb6c23a0b811d1509eefee", "7be5ede91ef16320b3942b0989ba7f5a", "bfc63e9d92422c78dd43421aff9d4f2a", "5304a9f0ad0d6ffdbe0f5892f3da3521", "6f52702045c7ec8f446715c21f694359",
	"e5f195b2e75db2da37b37621c4ccef74", "2cf5167355bd51138e68330e4f8fcb06", "a0791a0dedbff3c3309a8b6a236c6969", "8c4bac9ad0fbc2f558116020f0a5762f", "0716aceea326fe1ec77cf0c7341aab6c",
	"9c1f37ad011dc5f012da2747266bbbf9", "7d9084d927a07d0dbade428a42b7404a", "161c2dfbc2bf6f47692e7621febd4f9b", "6e409ce5e7336836446c9f2d464b011d", "e53c11db1d66d549847c4bb82b290134",
	"d350df08add4af570b740c13c472c421", "f825e741fc57b4f1788d2829d77f91e6", "a2c50d19d814590ccf3d0093e83a7bba", "fd04b1681c37db41a5d0686b1662913b", "17d794694a0ffdd62479da999b1726ea",
	"99420eb70a7fb45372abe474a302f327", "41d9f399784bbf2ddb7e0706451229ea", "b94468b266ca254412eaebd29660430a", "cdaff76697697e69279add75fb8c3621", "74b79da2704d8e0a3bb999788ea39cc9",
	"bdb54d42a6e1024a2725b365cdd7b374", "42d0db230876681d8b41711d5c61728e", "9bafb1066a30dd5151e0d2fa45665993", "a869f43b47132ab4f200451e38142bfc", "38adc81011189803818fc4179f16c466",
	"0c9df6774c08f57e0dfe32ceddb062f8", "1ec4184022acdd52c10bbba0f66c25f0", "63e108681e761f503d01d3b8a7f16329", "29962460d85f8b45dd3da3c94de865df", "5f0ffac05c076fc623a5ff88e08746dd",
	"3ea6d8bc1d8da20c3e0949679d314ae9", "93641e2fe05c9156d8882bda591920cc", "fbd5237b3e2c86fde4e58cc6c6601cbd", "559053e3b2e8c9a4a7a8fe87048877d9", "c9e6af260d7ca13c0bb4c93da1f1c769"
};
static const size_t MD5_ValuesCount = sizeof(MD5_Values) / sizeof(MD5_Values[0]);

static void YuvFillSamples(unsigned char *ptr, size_t size, int index)
{
	for (size_t i = 0; i < size; ++i)
	{
		ptr[i] = i % (0xFF - index);
	}
}

static BOOL BuffEquals(const void* buff1Ptr, size_t buff1Size, const void* buff2Ptr, size_t buff2Size)
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
#define Md5BuffEquals(buff1Ptr, buff2Ptr) BuffEquals((buff1Ptr), VPX_MD5_STRING_SIZE, (buff2Ptr), VPX_MD5_STRING_SIZE)

int _tmain(int argc, _TCHAR* argv[])
{
	vpx_codec_enc_cfg_t cfg = {0};
	vpx_codec_ctx_t context = {0};
	vpx_codec_err_t vpx_ret;
	vpx_enc_frame_flags_t enc_flags = 0; // VPX_EFLAG_XXX
	vpx_image_t image;
	unsigned char *inPtr = NULL, *outPtr = NULL;
	vpx_codec_pts_t pts = 1;

	VPX_DEBUG_INFO("**** Start ****");

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
		const vpx_codec_cx_pkt_t *pkt;
		vpx_codec_iter_t iter;
		for (int i = 0; i < VPX_LOOP_COUNT; ++i) {
			YuvFillSamples(inPtr, VPX_INPUT_SIZE, i);
			if (!vpx_img_wrap(&image, VPX_IMG_FMT_I420, VPX_INPUT_WIDTH, VPX_INPUT_HEIGHT, 1, inPtr)) {
				VPX_DEBUG_ERROR("vpx_img_wrap failed");
				goto bail;
			}
			if ((vpx_ret = vpx_codec_encode(&context, &image, pts++, 1, flags, VPX_DL_REALTIME)) != VPX_CODEC_OK) {
				VPX_DEBUG_ERROR("vpx_codec_encode failed with error =%s", vpx_codec_err_to_string(vpx_ret));
				goto bail;
			}
			vpx_img_free(&image);
			iter = NULL;
			while ((pkt = vpx_codec_get_cx_data(&context, &iter))) {
				switch (pkt->kind) {
				case VPX_CODEC_CX_FRAME_PKT:
					{
#if VPX_MD5_COMPARE || VPX_MD5_PRINT
						vpx_md5string_t md5;
						if (vpx_md5compute((const char*)pkt->data.frame.buf, pkt->data.frame.sz, &md5) != 0) {
							VPX_DEBUG_ERROR("vpx_md5compute failed");
							goto bail;
						}
#if VPX_MD5_COMPARE
						if (i < MD5_ValuesCount) {
							if (Md5BuffEquals(md5, MD5_Values[i]) != TRUE) {
								VPX_DEBUG_ERROR("Md5BuffEquals(%d) failed", i);
								goto bail;
							}
						}
						else {
							VPX_DEBUG_ERROR("i(%d) >= MD5_ValuesCount(%u)", i, MD5_ValuesCount);
							goto bail;
						}
						
#endif
#if VPX_MD5_PRINT
						printf("\"%s\",", md5);
#endif
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
		}
	}




bail:
	vpx_codec_destroy(&context);

	if (inPtr) free(inPtr);
	if (outPtr) free(outPtr);

	VPX_DEBUG_INFO("**** Done!!!! ****");

	getchar();

	return 0;
}

