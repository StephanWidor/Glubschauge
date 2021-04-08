# Glubschauge

![](glubsch.gif)

Qt based app, playing around with OpenCV's face detection and FaceMarks:
Get big eyes or nose by opening your mouth :-)

# Build

Glubschauge can be compiled for Linux, Android, MacOS and Windows.

Before building, you need to get the submodule that builds OpenCV:

`git submodule update --init --recursive`

Opening the CMakeLists.txt with QtCreator, and just build and run should work for
all platforms (more or less, see below).

cmake will take some time when called for the first time because it triggers an
OpenCV build by the OpenCV submodule.

## Linux

Besides a recent compiler (gcc or clang) and Qt, you need cmake, tbb, and openmp. On Debian/Ubuntu:

`sudo apt-get install cmake libtbb-dev libomp-dev`

If you want the app to be able to save gif files, you need magick++:

`sudo apt-get install libmagick++-dev`

Then do

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/gcc_64
make -j$(nproc)
```

Tested on Ubuntu20.04 with Qt5.14 and 5.15 using gcc9.3 and clang10.

## Android

Besides Qt, you need Android sdk and ndk installed.

If using QtCreator, Android SDK and NDK paths need to be set in the preferences. Compiling with the
"Android...Multi-Abi" Kit seems to behave weird somehow. If cmake complains about something like
"Invalid Android STL: cxx_shared", you may try removing "-DANDROID_STL:STRING=cxx_shared" and setting
"-DANDROID_NATIVE_API_LEVEL:STRING=26" in the "Initial CMake parameters" of the build configuration.

On Linux, you can also try running the android_build.sh script. You will probably have to adapt the Qt
and Android SDK/NDK path inside. You might also want to adapt the "ANDROID_BUILD_ABI_*" values to build
for the ABIs you need.

Tested on Ubuntu20.04 with Qt5.15 using latest Android sdk/ndk.

## MacOS

Besides a recent apple-clang and Qt, you need cmake, tbb, and openmp installed.

Installing with homebrew:

`brew install cmake tbb libomp`

should do.

If you want the app to be able to save gif files, you need magick++:

`brew install imagemagick`

Then do

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/clang_64
cmake --build .
```

Tested on Catalina with Qt5.15 using AppleClang11.

## Windows

Besides a recent msvc compiler and Qt, you need cmake. If it is not installed with
Visual Studio, you can also get it from cmake.org.

Then do

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL\msvc2019_64
cmake --build . --config Release -j
PATH_TO_YOUR_QT_INSTALL\bin\windeployqt\ --release --qmldir ..\qml Release\Glubschauge.exe
```

Tested on Windows 10 using msvc from Visual Studio 16.

# TODO

- fix opening video files on Android
- Win and iOS version
