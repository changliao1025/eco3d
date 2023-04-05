cmake_minimum_required(VERSION 3.10)
project(eco3d CXX)

set(CMAKE_CXX_STANDARD 11)
message("CMAKE_CURRENT_SOURCE_DIR is = ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(eco3d_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/atmosphere.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/atmosphere.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/canopy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/canopy.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cascade.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cascade.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/column.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/column.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/conversion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/conversion.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/data.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/data.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/date.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/date.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/dimension.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_auxiliary.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_cascade.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_initialize.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_read.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_run.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_save.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_setup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/eco3d_stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/evapotranspiration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/evapotranspiration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/geology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/geology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/glacier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/glacier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/global.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/global.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/groundwater.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/groundwater.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hru.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hru.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/infiltration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/infiltration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/interception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/interception.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lake.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lake.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/land.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/land.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/landcover.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/landcover.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/litter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/litter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/litterfall.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/litterfall.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mathematics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mathematics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/microbe.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/microbe.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/miscellaneous.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/miscellaneous.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/photosynthesis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/photosynthesis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/precipitation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/precipitation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/radiation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/radiation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/reach.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/reach.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/respiration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/respiration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/root.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/root.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/segment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/segment.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/snow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/snow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/soil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/stem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/stem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/stream.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/surface_runoff.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/surface_runoff.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/system.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/system.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/temperature.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/temperature.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vegetation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vegetation.h)

add_executable(eco3d ${eco3d_srcs})

set(default_build_type Release)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message("build type = ${default_build_type}")

target_compile_options(eco3d PUBLIC)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
install(TARGETS eco3d RUNTIME DESTINATION ${INSTALL_DIR})

find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_link_libraries(eco3d -std=c++11)
