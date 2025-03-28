# Android NDK supports amd64/x86_64 only on Linux
FROM --platform=linux/amd64 debian:bookworm-slim

RUN apt-get --allow-releaseinfo-change update && apt-get install -y --no-install-recommends \
  ninja-build \
  build-essential \
  openjdk-17-jdk-headless \
  curl \
  unzip \
  cmake \
  meson \
  bash \
  nasm \
  pkg-config \
  make \
  git \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

# Install Android SDK 
ARG ANDROID_SDK_URI=https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip
ENV ANDROID_SDK=/android-sdk
RUN curl ${ANDROID_SDK_URI} -o ./android-sdk.zip
RUN mkdir ${ANDROID_SDK} && unzip -qq ./android-sdk.zip -d ${ANDROID_SDK} && rm ./android-sdk.zip

ARG NDK_VERSION=27.2.12479018
ENV ANDROID_NDK=${ANDROID_SDK}/ndk/${NDK_VERSION}
RUN installAndroidComponent() { yes | ${ANDROID_SDK}/cmdline-tools/bin/sdkmanager --sdk_root=${ANDROID_SDK} "$1" > /dev/null; } && \
  installAndroidComponent "ndk;${NDK_VERSION}"

# Get ffmpeg sources
WORKDIR /usr
ARG FFMPEG=ffmpeg-7.1.1
RUN curl -LO "https://www.ffmpeg.org/releases/${FFMPEG}.tar.bz2"
RUN tar xf ${FFMPEG}.tar.bz2 -C src && rm ${FFMPEG}.tar.bz2
RUN ln -s ${FFMPEG} /usr/src/ffmpeg

# Build
COPY build.sh ffmpeg_build.sh

ENV TARGET_ABI="aarch64"
ENV ANDROID_API_LEVEL=35
ENV AVCODEC_INSTALL_DIR=/usr/build/ffmpeg
ENV FFMPEG_SRC=/usr/src/${FFMPEG}

CMD [ "./ffmpeg_build.sh", "/usr/src/ffmpeg" ]
