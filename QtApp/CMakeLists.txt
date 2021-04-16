cmake_minimum_required(VERSION 3.16)
project(Glubschauge LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    set(ANDROID_PERMISSIONS "android.permission.READ_EXTERNAL_STORAGE android.permission.WRITE_EXTERNAL_STORAGE android.permission.CAMERA"
        CACHE INTERNAL "")
    set(ANDROID_NATIVE_API_LEVEL 26)
    set(ANDROID_MIN_SDK_VERSION 26)
    set(ANDROID_TARGET_SDK_VERSION 30)

    find_package(Qt5 COMPONENTS AndroidExtras Core Quick Multimedia Svg Widgets REQUIRED)

    # On Android, static linking of parallel projects doesn't work somehow.
    # To get around this, we add them as subdirectories, which works (whyever...)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../GlubschLib ${CMAKE_CURRENT_BINARY_DIR}/GlubschLib) # swdebug
else()
    find_package(Qt5 COMPONENTS Core Quick Multimedia Svg Widgets REQUIRED)
endif()

set(HEADERS
    qt/Application.h
    qt/Assets.h
    qt/FileSystem.h
    qt/ImageConvert.h
    qt/ImageTransform.h
    qt/ProcessingFilter.h
    qt/VideoChooser.h
    )
set(SOURCES
    main.cpp
    qt/Application.cpp
    qt/Assets.cpp
    qt/FileSystem.cpp
    qt/ImageConvert.cpp
    qt/ImageTransform.cpp
    qt/ProcessingFilter.cpp
    qt/VideoChooser.cpp
    )
set(RESOURCES
    appdata.qrc
    icons.qrc
    qml.qrc
    )

if(ANDROID)
    add_compile_options(-fopenmp)
    add_link_options(-fopenmp -static-openmp)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES})
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt5::AndroidExtras
        )
else()
    add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    GlubschLib
    Qt5::Core
    Qt5::Gui
    Qt5::Multimedia
    Qt5::Quick
    Qt5::Svg
    )
