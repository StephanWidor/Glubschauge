# Glubschauge

![](glubsch.gif)

Qt based app, playing around with OpenCV's face detection and FaceMarks:
Get big eyes or nose by opening your mouth :-)

# Build

Glubschauge can be compiled for Linux, Android, MacOS and Windows.
(Although only tested on Arch Linux desktop build, as well as cross compiled for Android)

Before building, you need to get the submodule that builds OpenCV:

`git submodule update --init --recursive`

Opening the CMakeLists.txt with QtCreator, and just build and run should work for
all platforms (more or less, see below).

cmake will take some time when called for the first time because it triggers an
OpenCV build by the OpenCV submodule.

## Linux

A recent compiler, Qt6 and cmake is needed, optionally OpenMP.
If you want the app to be able to save gif files, you also need magick++ (on arch linux imagemagick package, on debian/ubuntu libmagick++-dev)

Then do

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/gcc_64 ..
cmake --build . -j
```

On Linux, you can also stream to an output device. If you have v4l2loopback installed
(on Debian/Ubuntu deb packages v4l2loopback-dkms and v4l2loopback-utils, same on Arch Linux),
do e.g.

```
sudo modprobe v4l2loopback exclusive_caps=1 video_nr=5 card_label="Glubschauge Cam"
```

Then choose `/device/video5` as output device in the app settings, and enjoy using Glubschauge
as camera in other applications.


Tested on Arch Linux with Qt6.8.3, using gcc14.2.1 and clang19.1.7.

## Android

Besides Qt6, you need Android sdk and ndk installed.

If using QtCreator, Android SDK and NDK paths need to be set in the preferences.
Don't really know why, but in the "Initial Configuration" of the build settings you should set ANDROID_PLATFORM to something like android-30.

On Linux, you can also try running the android_build.sh script. You will probably have to adapt some of the variables set in the beginning of the script.
The script will create an apk, which you still have to sign before you can install it on a device:
```
./android_build.sh
cd build.android/arm64-v8a/QtApp/android-build
keytool -genkey -v -keystore glubschauge.keystore -keyalg RSA -keysize 2048 -validity 10000 -alias glubschauge
/opt/android-sdk/build-tools/36.0.0/apksigner sign --ks-key-alias glubschauge --ks glubschauge.keystore Glubschauge.apk
```
(assuming you have your android-sdk in that place, and built Glubschauge for arm64-v8a)

After enabling usb debugging on your phone and connecting it to your computer, you can then install
via adb:
```
adb install Glubschauge.apk
```
Or copy the apk to your phone and install from there.

Tested on Arch Linux with Qt6.8.3, installed on a Pixel 7a, Android 15.

## MacOS

Not tested, but might well work doing something like

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL/clang_64 ..
cmake --build . -j
```

## Windows

Tested on Windows 11 using VS 2022

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_INSTALL\msvc2022_64 ..
cmake --build . --config Release -j
PATH_TO_YOUR_QT_INSTALL\bin\windeployqt --release --qmldir ..\QtApp\qml QtApp\Release\Glubschauge.exe
```

# Known Issues

- Permissions for file saving on recent Android are a bit special, and it seems like not yet really covered by Qt6. That's why - on Android - capture images are saved intermediately in the app specific storage, followed by a file dialog being opened that lets you choose where to move that file (like e.g. in global pictures folder).
- For some video formats (well... maybe rather a lot), the accessible QFrameBuffer doesn't seem to be mappable to CPU memory, which has the consequence that Glubschauge cannot process them.
