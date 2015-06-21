/*
 *  Copyright (c) 2011 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include "vpx_config.h"
#if !defined(RTCD_C)
#define RTCD_C
#endif
#include "vpx_scale_rtcd.h"
#include "vp8_rtcd.h" // FIXME: Mamadou
#include "vpx_ports/vpx_once.h"

void vpx_scale_rtcd()
{
    once(setup_rtcd_internal);
}
