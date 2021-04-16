cmake_minimum_required(VERSION 3.16)
project(Glubschauge-main LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    set(ANDROID_NATIVE_API_LEVEL 26)
    set(ANDROID_MIN_SDK_VERSION 26)
    set(ANDROID_TARGET_SDK_VERSION 30)
endif()

# On Android, static linking of parallel projects doesn't work somehow.
# To get around this, we add them as subdirectories, which works (whyever...)
if(NOT ANDROID)
    add_subdirectory(OpenCV)
    add_subdirectory(GlubschLib)
endif()
add_subdirectory(QtApp)
