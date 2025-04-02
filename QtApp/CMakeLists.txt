cmake_minimum_required(VERSION 3.25)
project(Glubschauge LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Multimedia Quick Widgets)

qt_standard_project_setup(REQUIRES 6.8)

set(SOURCES
    main.cpp
    qt/Assets.cpp
    qt/Assets.h
    qt/FileSystem.cpp
    qt/FileSystem.h
    qt/ImageCapture.cpp
    qt/ImageCapture.h
    qt/ProcessingFilter.cpp
    qt/ProcessingFilter.h
)
qt_add_resources(SOURCES
    appdata.qrc
    icons.qrc
    qml.qrc
)

qt_add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    GlubschLib
    Qt6::Core
    Qt6::Multimedia
    Qt6::Quick
    Qt6::Widgets
)

if(ANDROID)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
        QT_ANDROID_PACKAGE_NAME "com.stephanwidor.glubschauge"
    )
    set(ANDROID_VERSION_CODE 2)
    set(ANDROID_VERSION_NAME 1.1.0)
endif()
