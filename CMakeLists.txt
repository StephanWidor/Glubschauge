cmake_minimum_required(VERSION 3.5)
project(Glubschauge LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    cv/Utils2D.h
    FileSystem.h
    FpsCounter.h
    Logger.h
    magick/GifContainer.h
    NumberSmoothing.h
    qt/Application.h
    qt/Assets.h
    qt/FileSystem.h
    qt/ImageConvert.h
    qt/ImageTransform.h
    qt/Logger.h
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
    #set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    set(ANDROID_ABIS "armeabi-v7a arm64-v8a x86 x86_64" CACHE STRING "")
    set(ANDROID_BUILD_ABI_armeabi-v7a ON CACHE BOOL "")
    #set(ANDROID_BUILD_ABI_arm64-v8a ON CACHE BOOL "")
    #set(ANDROID_BUILD_ABI_x86 ON CACHE BOOL "")
    #set(ANDROID_BUILD_ABI_x86_64 ON CACHE BOOL "")
    set(ANDROID_NATIVE_API_LEVEL 26 CACHE STRING "")
    set(ANDROID_API_VERSION 26 CACHE STRING "")
    set(ANDROID_MIN_SDK_VERSION 26 CACHE STRING "")
    set(ANDROID_TARGET_SDK_VERSION  26 CACHE STRING "")
    set(ANDROID_PERMISSIONS "android.permission.READ_EXTERNAL_STORAGE android.permission.WRITE_EXTERNAL_STORAGE android.permission.CAMERA"
        CACHE INTERNAL "")
    set(OPENCV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/android/OpenCV-android-4.3.0-contrib/native")
    set(MAGICK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/android/ImageMagick")
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/android/include
        ${OPENCV_PATH}/jni/include
        ${MAGICK_PATH}/include
        )
    find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)
    add_compile_options(-fopenmp)# -static-openmp)
    add_compile_definitions(MAGICKCORE_HDRI_ENABLE=0 MAGICKCORE_QUANTUM_DEPTH=8)
    add_link_options(-fopenmp -static-openmp)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES})
    set(OPENCVSTATICLIBS_PATH "${OPENCV_PATH}/staticlibs/${ANDROID_ABI}")
    set(OPENCV3RDPARTYLIBS_PATH "${OPENCV_PATH}/3rdparty/libs/${ANDROID_ABI}")
    set(MAGICKLIBS_PATH "${MAGICK_PATH}/lib/${ANDROID_ABI}")
    target_link_libraries(Glubschauge
        PUBLIC
        ${OPENCVSTATICLIBS_PATH}/libopencv_face.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_imgcodecs.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_imgproc.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_objdetect.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_core.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibtiff.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibjpeg-turbo.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibjasper.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibpng.a
        ${OPENCV3RDPARTYLIBS_PATH}/libtbb.a
        ${OPENCV3RDPARTYLIBS_PATH}/libIlmImf.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibwebp.a
        ${OPENCV3RDPARTYLIBS_PATH}/libcpufeatures.a
        ${OPENCV3RDPARTYLIBS_PATH}/libittnotify.a
        ${OPENCV3RDPARTYLIBS_PATH}/libtbb.a
        ${MAGICKLIBS_PATH}/libMagick++-7.Q8.a
        ${MAGICKLIBS_PATH}/libMagickWand-7.Q8.a
        ${MAGICKLIBS_PATH}/libMagickCore-7.Q8.a
        log
        GLESv2
        z
        Qt5::AndroidExtras
        )
    if(ANDROID_ABI MATCHES "arm*")
        target_link_libraries(Glubschauge
            PUBLIC
            ${OPENCV3RDPARTYLIBS_PATH}/libtegra_hal.a
            )
    else()
        target_link_libraries(Glubschauge
            PUBLIC
            ${OPENCV3RDPARTYLIBS_PATH}/libippicv.a
            ${OPENCV3RDPARTYLIBS_PATH}/libippiw.a
            )
    endif()
elseif(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MAGICK++ REQUIRED IMPORTED_TARGET Magick++)
    pkg_check_modules(OPENCV4 REQUIRED IMPORTED_TARGET opencv4)
    target_link_libraries(Glubschauge
      PRIVATE
      PkgConfig::MAGICK++
      PkgConfig::OPENCV4
      tbb
      )
else()
    include_directories(/usr/include/ImageMagick-6)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MAGICK++ REQUIRED IMPORTED_TARGET Magick++)
    pkg_check_modules(OPENCV4 REQUIRED IMPORTED_TARGET opencv4)
    pkg_check_modules(TBB REQUIRED IMPORTED_TARGET tbb)
    target_link_libraries(Glubschauge
      PRIVATE
      PkgConfig::MAGICK++
      PkgConfig::OPENCV4
      PkgConfig::TBB
      pthread
      )
endif()

find_package(Qt5 COMPONENTS Core Quick Multimedia Svg Widgets REQUIRED)
target_link_libraries(Glubschauge
    PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::Svg Qt5::Widgets)
