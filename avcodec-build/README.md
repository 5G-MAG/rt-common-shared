# Scripts to build ffmpeg libraries for Android

Helper script to build ffmpeg libraries for Android.
The script is intended for use on Linux & Mac OSX. 
On Windows, building is supported through WSL, or else using the Dockerfile. In Windows, please use PowerShell (not Git Bash).


***build.sh* usage :**

```
# Get ffmpeg sources
curl -LO "https://www.ffmpeg.org/releases/ffmpeg-7.1.1.tar.bz2"
tar xf ffmpeg-7.1.1.tar.bz2 -C src && rm ffmpeg-7.1.1.tar.bz2

# Configure NDK path
export ANDROID_SDK=/path/to/android-sdk
export ANDROID_NDK=${ANDROID_SDK}/ndk/27.2.12479018

# Configure build target
export TARGET_ABI=aarch64
export ANDROID_API_LEVEL=35
export AVCODEC_INSTALL_DIR=build/ffmpeg/aarch64

# Build
./build.sh src/ffmpeg-7.1.1
```


***Dockerfile* usage :** 

**Linux (x86-64)**
```
docker build -t ffmpeg-builder:27 --build-arg NDK_VERSION=27.2.12479018 .
 
docker run -v $(pwd)/build/ffmpeg/aarch64:/usr/build/ffmpeg --env TARGET_ABI=aarch64 --env ANDROID_API_LEVEL=35 ffmpeg-builder:27
```

**Windows (x86-64)**
```
docker build -t ffmpeg-builder:27 --build-arg NDK_VERSION=27.2.12479018 .
 
docker run -v ${pwd}/build/ffmpeg/aarch64:/usr/build/ffmpeg --env TARGET_ABI=aarch64 --env ANDROID_API_LEVEL=35 ffmpeg-builder:27
```

**On arm64 hosts, such as Mac OSX on Apple silicon**
```
podman build -t ffmpeg-builder:27 --build-arg NDK_VERSION=27.2.12479018 .
 
podman run -v ${pwd}/build/ffmpeg/aarch64:/usr/build/ffmpeg --env TARGET_ABI=aarch64 --env ANDROID_API_LEVEL=35 ffmpeg-builder:27
```


## Additional recommendations in relation to the V3C Immersive Platform

### Android
Once compiled, the `.so` libraries from the build artifacts' `lib` directory can be included into the `./External/avcodec/7.1/Android/arm64-v8a/lib` directory of the rt-v3c-unity-player.

