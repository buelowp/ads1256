cmake_minimum_required(VERSION 3.0)
project(ads1256)

option(BUILD_SHARED_LIB "Build as a shared object (default ON)" ON)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

set(CMAKE_CXX_STANDARD 14)

set(PROJ_VERSION_MAJOR 1)
set(PROJ_VERSION_MINOR 0)
set(PROJ_VERSION_PATCH 0)
set(PROJ_VERSION ${PROJ_VERSION_MAJOR}.${PROJ_VERSION_MINOR}.${PROJ_VERSION_PATCH} )

if(BUILD_SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJ_VERSION} SOVERSION ${PROJ_VERSION})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} -lwiringPi)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

install(TARGETS ${PROJECT_NAME} DESTINATION lib) 
install(FILES ${HEADERS} DESTINATION include)

