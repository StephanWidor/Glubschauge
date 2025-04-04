cmake_minimum_required(VERSION 3.25)
project(Glubschauge-main LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_QT_APP "Build Qt App requiring Qt dev framework installed" ON)
include(CMakeDependentOption)
cmake_dependent_option(BUILD_OCV_HIGHGUI_APP "Build App using OCV's highgui module" ON
                       "NOT ANDROID" OFF)

if(WIN32)
    set(OCV_FORCE_RELEASE_BUILD "" CACHE STRING OFF)    # otherwise we get problems with CRT
endif()
add_subdirectory(OpenCV)
add_subdirectory(GlubschLib)

if(BUILD_QT_APP)
    add_subdirectory(QtApp)
endif()
if(BUILD_OCV_HIGHGUI_APP)
    add_subdirectory(OCVHighGuiApp)
endif()
