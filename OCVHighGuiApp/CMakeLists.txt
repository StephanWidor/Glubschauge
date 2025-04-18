cmake_minimum_required(VERSION 3.25)
project(OCVHighGuiApp LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    )
set(SOURCES
    main.cpp
    )

file(READ ../appData/haarcascade_frontalface_default.xml HAARCASCADE_XML)
file(READ ../appData/lbfmodel.yaml LBFMODEL_YAML)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource_help.h.in ${CMAKE_BINARY_DIR}/resource_help.h @ONLY)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    GlubschLib
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
