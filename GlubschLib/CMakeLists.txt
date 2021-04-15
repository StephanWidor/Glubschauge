cmake_minimum_required(VERSION 3.16)
project(GlubschLib LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
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
    )

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

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
target_link_libraries(${PROJECT_NAME} PUBLIC OpenCV)

##### ImageMagick #####
if(ANDROID)
    set(MAGICK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../android/ImageMagick")
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
