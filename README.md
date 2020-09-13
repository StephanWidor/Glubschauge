# Glubschauge

![](glubsch.gif)

Qt based app, playing around with OpenCV's face detection and FaceMarks:
Get big eyes or nose by opening your mouth :-)

# Build

## Linux

Besides Qt, you need cmake, pkgconfig, tbb, opencv, magick++ installed.

`sudo apt-get install cmake pkg-config libtbb-dev libopencv-dev libmagick++-dev`

should do on Debian/Ubuntu.

You can open CMakeLists.txt with QtCreator, or do

`cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/gcc_64 && make -j`

Tested on Ubuntu20.04 with Qt5.14 and 5.15 using gcc9.3 and clang10.

## Android

Besides Qt, you need Android sdk and ndk installed and set up in QtCreator.

If you open CMakeLists.txt with QtCreator, you should be able to compile with "Android...Multi-Abi" Kit.
If cmake complains about something like "Invalid Android STL: cxx_shared", remove
"-DANDROID_STL:STRING=cxx_shared" from the "Initial CMake parameters" of the build configuration.

Tested on Ubuntu20.04 with Qt5.15 using latest Android sdk/ndk.

## MacOS

Besides Qt, you need cmake, pkgconfig, tbb, opencv, magick++ installed.

Installing with homebrew:

`brew install cmake pkg-config tbb opencv imagemagick`

should do.

You can open CMakeLists.txt with QtCreator, or do

`cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/clang_64 && cmake --build . -j`

Tested on Catalina with Qt5.15 using AppleClang11.


# TODO

- fix opening video files on Android
- multi ABI build for Android
- make compile on Windows, MacOS, iOS
- package/deploy
