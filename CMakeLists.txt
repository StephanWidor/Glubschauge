cmake_minimum_required(VERSION 3.16)
project(Glubschauge LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(OpenCV)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    Assets.h
    cv/BarrelCreation.h
    cv/FaceDetection.h
    cv/FaceDistortionType.h
    cv/FlashEffect.h
    cv/FpsEffect.h
    cv/GifCreate.h
    cv/GlubschEffect.h
    cv/ImageUtils.h
    cv/MouthOpen.h
    cv/OutputDevice.h
    cv/Utils2D.h
    FpsCounter.h
    Logger.h
    magick/GifContainer.h
    NumberSmoothing.h
    qt/Application.h
    qt/Assets.h
    qt/FileSystem.h
    qt/ImageConvert.h
    qt/ImageTransform.h
    qt/ProcessingFilter.h
    qt/VideoChooser.h
    )
set(SOURCES
    cv/BarrelCreation.cpp
    cv/FaceDetection.cpp
    cv/FlashEffect.cpp
    cv/FpsEffect.cpp
    cv/GifCreate.cpp
    cv/GlubschEffect.cpp
    cv/ImageUtils.cpp
    cv/OutputDevice.cpp
    FpsCounter.cpp
    magick/GifContainer.cpp
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

##### OpenMP #####
if(ANDROID)
    add_compile_options(-fopenmp)
    add_link_options(-fopenmp -static-openmp)
else()
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()

##### OpenCV #####
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCV)

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

##### ImageMagick #####
if(ANDROID)
    set(MAGICK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/android/ImageMagick")
    include_directories(
        ${MAGICK_PATH}/include
        )
    add_compile_definitions(MAGICKCORE_HDRI_ENABLE=0 MAGICKCORE_QUANTUM_DEPTH=8 IMAGEMAGICK_AVAILABLE)
    set(MAGICKLIBS_PATH "${MAGICK_PATH}/lib/${ANDROID_ABI}")
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${MAGICKLIBS_PATH}/libMagick++-7.Q8.a
        ${MAGICKLIBS_PATH}/libMagickWand-7.Q8.a
        ${MAGICKLIBS_PATH}/libMagickCore-7.Q8.a
        )
else()
    find_package(ImageMagick COMPONENTS Magick++ MagickWand MagickCore)
    if(ImageMagick_FOUND)
        message("Found ImageMagick ${ImageMagick_VERSION_STRING}, will enable gif export")
        add_compile_definitions(MAGICKCORE_HDRI_ENABLE=0 MAGICKCORE_QUANTUM_DEPTH=16 IMAGEMAGICK_AVAILABLE)
        include_directories(${ImageMagick_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}
          PRIVATE
          ${ImageMagick_LIBRARIES}
          )
    else()
        message("ImageMagick not found, will disable gif export")
    endif()
endif()
