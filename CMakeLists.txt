cmake_minimum_required(VERSION 3.16)
project(Glubschauge LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(OpenCV)
add_subdirectory(GlubschLib)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
    appData/appdata.qrc
    icons/icons.qrc
    qml/qml.qrc
    )

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    set(ANDROID_PERMISSIONS "android.permission.READ_EXTERNAL_STORAGE android.permission.WRITE_EXTERNAL_STORAGE android.permission.CAMERA"
        CACHE INTERNAL "")

    set(ANDROID_NATIVE_API_LEVEL 26)
    set(ANDROID_MIN_SDK_VERSION 26)
    set(ANDROID_TARGET_SDK_VERSION 30)

    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES})
elseif(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
endif()

##### GlubschLib #####
target_link_libraries(${PROJECT_NAME} PRIVATE GlubschLib)

##### Qt #####
if(ANDROID)
    find_package(Qt5 COMPONENTS AndroidExtras Core Quick Multimedia Svg Widgets REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::AndroidExtras Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::Svg Qt5::Widgets)
else()
    find_package(Qt5 COMPONENTS Core Quick Multimedia Svg Widgets REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::Svg Qt5::Widgets)
endif()
