#!/usr/bin/env bash

if [ -z "$1" ]; then
  echo "Usage: ./build.sh FFMPEG_SRC_DIR"
  exit 1
fi

FFMPEG_SRC_DIR=$1

if [ -z "$ANDROID_API_LEVEL" ]; then
  ANDROID_API_LEVEL=35
fi

if [ -z "$TARGET_ABI" ]; then
  TARGET_ABI="aarch64"
fi

if [ -z "$AVCODEC_INSTALL_DIR" ]; then
  AVCODEC_INSTALL_DIR=$(pwd)/build/ffmpeg/$TARGET_ABI
fi

case "$OSTYPE" in
  darwin*)  NDK_HOST="darwin-x86_64" ;;
  linux*)   NDK_HOST="linux-x86_64" ;;
  msys*)    NDK_HOST="windows-x86_64" ;;
  *)        echo "unknown host type: \"$OSTYPE\"" && exit 1;;
esac

case ${TARGET_ABI} in
  armv7-a)
    TOOLCHAIN=armv7a-linux-androideabi
    ;;
  aarch64)
    TOOLCHAIN=aarch64-linux-android
    ;;
  x86)
    TOOLCHAIN=i686-linux-android
    EXTRA_CONFIG="--disable-asm"
    ;;
  x86_64)
    TOOLCHAIN=x86_64-linux-android
    ARCH=x86_64
    EXTRA_CONFIG="--disable-asm"
    ;;
esac

NDK_TOOLCHAIN_PATH=$ANDROID_NDK/toolchains/llvm/prebuilt/$NDK_HOST
CC=$NDK_TOOLCHAIN_PATH/bin/${TOOLCHAIN}${ANDROID_API_LEVEL}-clang
MAKE_EXECUTABLE=$ANDROID_NDK/prebuilt/$NDK_HOST/bin/make

if [ ! -d "$FFMPEG_SRC_DIR" ]; then
  echo "not a directory: $FFMPEG_SRC_DIR"
  exit 1
fi

if [ ! -d "$AVCODEC_INSTALL_DIR" ]; then
  mkdir -p $AVCODEC_INSTALL_DIR
fi

AVCODEC_INSTALL_DIR=$(readlink -f $AVCODEC_INSTALL_DIR)
cd $FFMPEG_SRC_DIR

./configure \
  --prefix=$AVCODEC_INSTALL_DIR \
  --enable-cross-compile \
  --target-os=android \
  --arch=$TARGET_ABI \
  --sysroot=$NDK_TOOLCHAIN_PATH/sysroot \
  --cc=$CC \
  --cxx=$CC++ \
  --ld=$CC \
  --ar=$NDK_TOOLCHAIN_PATH/bin/llvm-ar \
  --as=$CC \
  --nm=$NDK_TOOLCHAIN_PATH/bin/llvm-nm \
  --ranlib=$NDK_TOOLCHAIN_PATH/bin/llvm-ranlib \
  --strip=$NDK_TOOLCHAIN_PATH/bin/llvm-strip \
  --extra-cflags="-O3 -fPIC" \
  --extra-ldflags="-Wl,-z,max-page-size=16384" \
  --pkg-config=/usr/bin/pkg-config \
  --enable-shared \
  --disable-static \
  --disable-programs \
  --disable-doc \
  --enable-jni \
  --enable-mediacodec \
  --enable-decoder=mp3_mediacodec \
  --enable-decoder=aac_mediacodec \
  --enable-decoder=h264_mediacodec \
  --enable-decoder=hevc_mediacodec \
  ${EXTRA_CONFIG}
  
$MAKE_EXECUTABLE clean
$MAKE_EXECUTABLE -j$(nproc)
$MAKE_EXECUTABLE install

# when making changes to the build configuration, 
# make sure the license here reflects the build configuration
cp ./COPYING.LGPLv2.1 ${AVCODEC_INSTALL_DIR}/LICENSE