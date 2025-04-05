cmake_minimum_required(VERSION 3.25)
project(GlubschLib LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    cv/FaceDetection.h
    cv/FaceDistortion.h
    cv/FlashEffect.h
    cv/FpsEffect.h
    cv/GlubschEffect.h
    cv/ImageUtils.h
    cv/MouthOpen.h
    cv/SaveUtils.h
    cv/Utils2D.h
    cv/V4lLoopbackDevice.h
    FpsCounter.h
    logger.h
    magick/GifContainer.h
    NumberSmoothing.h
    spin_mutex.h
    )
set(SOURCES
    cv/FaceDetection.cpp
    cv/FaceDistortion.cpp
    cv/FlashEffect.cpp
    cv/FpsEffect.cpp
    cv/GlubschEffect.cpp
    cv/ImageUtils.cpp
    cv/SaveUtils.cpp
    cv/V4lLoopbackDevice.cpp
    FpsCounter.cpp
    magick/GifContainer.cpp
    )

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME}
   PUBLIC
   BuildOpenCV
   )

##### OpenMP #####
if(ANDROID)
    # find_package doesn't seem to work on android
    add_compile_options(-fopenmp)
    add_link_options(-fopenmp -static-openmp)
else()
    find_package(OpenMP COMPONENTS CXX)
    if(OpenMP_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    endif()
endif()

##### ImageMagick #####
if(ANDROID)
    set(MAGICK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../ImageMagick/android")
    include_directories(
        ${MAGICK_PATH}/include
        )
    target_compile_definitions(${PROJECT_NAME} PUBLIC MAGICKCORE_HDRI_ENABLE=0 MAGICKCORE_QUANTUM_DEPTH=8 IMAGEMAGICK_AVAILABLE)
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
        target_compile_definitions(${PROJECT_NAME} PUBLIC MAGICKCORE_HDRI_ENABLE=0 MAGICKCORE_QUANTUM_DEPTH=16 IMAGEMAGICK_AVAILABLE)
        include_directories(${ImageMagick_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}
          PRIVATE
          ${ImageMagick_LIBRARIES}
          )
    else()
        message("ImageMagick not found, will disable gif export")
    endif()
endif()
