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
    cv/GlubschEffect.h
    cv/ImageUtils.h
    cv/MouthOpen.h
    cv/Utils2D.h
    FileSystem.h
    FpsCounter.h
    Logger.h
    magick/GifCreate.h
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
    cv/GlubschEffect.cpp
    cv/ImageUtils.cpp
    FpsCounter.cpp
    magick/GifCreate.cpp
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
    set(OPENCV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/android/OpenCV-android-4.3.0-contrib/native")
    set(OPENCVSHAREDLIBS_PATH "${OPENCV_PATH}/libs/${ANDROID_ABI}")
    include_directories(
        ${OPENCV_PATH}/jni/include
        ${CMAKE_CURRENT_SOURCE_DIR}/android/include
        )
    set(ANDROID_EXTRA_LIBS
        ${OPENCVSHAREDLIBS_PATH}/libopencv_java4.so
        CACHE INTERNAL ""
        )
    find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES})
    set(OPENCVSTATICLIBS_PATH "${OPENCV_PATH}/staticlibs/${ANDROID_ABI}")
    set(OPENCV3RDPARTYLIBS_PATH "${OPENCV_PATH}/3rdparty/libs/${ANDROID_ABI}")
    target_link_libraries(Glubschauge
        PUBLIC
        ${OPENCVSTATICLIBS_PATH}/libopencv_face.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_imgcodecs.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_flann.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_imgproc.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_highgui.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_features2d.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_calib3d.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_ml.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_objdetect.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_photo.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_stitching.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_superres.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_video.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_videostab.a
        ${OPENCVSTATICLIBS_PATH}/libopencv_core.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibtiff.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibjpeg-turbo.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibjasper.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibpng.a
        ${OPENCV3RDPARTYLIBS_PATH}/libtbb.a
        ${OPENCV3RDPARTYLIBS_PATH}/libIlmImf.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibwebp.a
        ${OPENCV3RDPARTYLIBS_PATH}/libcpufeatures.a
        ${OPENCV3RDPARTYLIBS_PATH}/liblibprotobuf.a
        ${OPENCV3RDPARTYLIBS_PATH}/libittnotify.a
        ${OPENCV3RDPARTYLIBS_PATH}/libtegra_hal.a
        ${OPENCV3RDPARTYLIBS_PATH}/libtbb.a
        log
        GLESv2
        z
        Qt5::AndroidExtras
        )
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