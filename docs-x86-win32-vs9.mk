## Copyright (c) 2011 The WebM project authors. All Rights Reserved.
## 
## Use of this source code is governed by a BSD-style license
## that can be found in the LICENSE file in the root of the source
## tree. An additional intellectual property rights grant can be found
## in the file PATENTS.  All contributing project authors may
## be found in the AUTHORS file in the root of the source tree.
# This file automatically generated by configure. Do not edit!
SRC_PATH="/cygdrive/c/Projects/libvpx"
SRC_PATH_BARE=/cygdrive/c/Projects/libvpx
BUILD_PFX=
TOOLCHAIN=x86-win32-vs9
ASM_CONVERSION=/cygdrive/c/Projects/libvpx/build/make/ads2gas.pl
GEN_VCPROJ=/cygdrive/c/Projects/libvpx/build/make/gen_msvs_proj.sh
MSVS_ARCH_DIR=x86-msvs

CC=
CXX=
AR=
LD=
AS=msvs
STRIP=
NM=

CFLAGS  = 
CXXFLAGS  = 
ARFLAGS = -rus$(if $(quiet),c,v)
LDFLAGS = 
ASFLAGS =  -f win32
extralibs = 
AS_SFX    = .asm
EXE_SFX   = .exe
VCPROJ_SFX = vcproj
RTCD_OPTIONS = --disable-avx --disable-avx2 
fmt_deps = sed -e 's;^\([a-zA-Z0-9_]*\)\.o;${@:.d=.o} $@;'
ARCH_X86=yes
HAVE_MMX=yes
HAVE_SSE=yes
HAVE_SSE2=yes
HAVE_SSE3=yes
HAVE_SSSE3=yes
HAVE_SSE4_1=yes
HAVE_VPX_PORTS=yes
CONFIG_EXTERNAL_BUILD=yes
CONFIG_INSTALL_BINS=yes
CONFIG_INSTALL_LIBS=yes
CONFIG_USE_X86INC=yes
CONFIG_MSVS=yes
CONFIG_FAST_UNALIGNED=yes
CONFIG_MD5=yes
CONFIG_RUNTIME_CPU_DETECT=yes
CONFIG_POSTPROC=yes
CONFIG_MULTITHREAD=yes
CONFIG_VP8_ENCODER=yes
CONFIG_VP8_DECODER=yes
CONFIG_VP9_ENCODER=yes
CONFIG_VP9_DECODER=yes
CONFIG_VP8=yes
CONFIG_VP9=yes
CONFIG_ENCODERS=yes
CONFIG_DECODERS=yes
CONFIG_STATIC_MSVCRT=yes
CONFIG_SPATIAL_RESAMPLING=yes
CONFIG_REALTIME_ONLY=yes
CONFIG_ERROR_CONCEALMENT=yes
CONFIG_STATIC=yes
CONFIG_OS_SUPPORT=yes
CONFIG_UNIT_TESTS=yes
CONFIG_TEMPORAL_DENOISING=yes
CONFIG_VS_VERSION=9
