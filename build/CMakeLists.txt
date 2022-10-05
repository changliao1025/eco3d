cmake_minimum_required(VERSION 3.10)
project(eco3d CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(../eco3d/src)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB eco3d_srcs
        ../eco3d/src/atmosphere.cpp
        ../eco3d/src/atmosphere.h
        ../eco3d/src/canopy.cpp
        ../eco3d/src/canopy.h
        ../eco3d/src/cascade.cpp
        ../eco3d/src/cascade.h
        ../eco3d/src/column.cpp
        ../eco3d/src/column.h
        ../eco3d/src/conversion.cpp
        ../eco3d/src/conversion.h
        ../eco3d/src/data.cpp
        ../eco3d/src/data.h
        ../eco3d/src/date.cpp
        ../eco3d/src/date.h
        ../eco3d/src/dimension.h
        ../eco3d/src/eco3d.h
        ../eco3d/src/eco3d_auxiliary.cpp
        ../eco3d/src/eco3d_cascade.cpp
        ../eco3d/src/eco3d_initialize.cpp
        ../eco3d/src/eco3d_main.cpp
        ../eco3d/src/eco3d_read.cpp
        ../eco3d/src/eco3d_run.cpp
        ../eco3d/src/eco3d_save.cpp
        ../eco3d/src/eco3d_setup.cpp
        ../eco3d/src/eco3d_stream.cpp
        ../eco3d/src/evapotranspiration.cpp
        ../eco3d/src/evapotranspiration.h
        ../eco3d/src/geology.cpp
        ../eco3d/src/geology.h
        ../eco3d/src/glacier.cpp
        ../eco3d/src/glacier.h
        ../eco3d/src/global.cpp
        ../eco3d/src/global.h
        ../eco3d/src/groundwater.cpp
        ../eco3d/src/groundwater.h
        ../eco3d/src/hru.cpp
        ../eco3d/src/hru.h
        ../eco3d/src/infiltration.cpp
        ../eco3d/src/infiltration.h
        ../eco3d/src/interception.cpp
        ../eco3d/src/interception.h
        ../eco3d/src/lake.cpp
        ../eco3d/src/lake.h
        ../eco3d/src/land.cpp
        ../eco3d/src/land.h
        ../eco3d/src/landcover.cpp
        ../eco3d/src/landcover.h
        ../eco3d/src/litter.cpp
        ../eco3d/src/litter.h
        ../eco3d/src/litterfall.cpp
        ../eco3d/src/litterfall.h
        ../eco3d/src/main.cpp
        ../eco3d/src/mathematics.cpp
        ../eco3d/src/mathematics.h
        ../eco3d/src/microbe.cpp
        ../eco3d/src/microbe.h
        ../eco3d/src/miscellaneous.cpp
        ../eco3d/src/miscellaneous.h
        ../eco3d/src/photosynthesis.cpp
        ../eco3d/src/photosynthesis.h
        ../eco3d/src/precipitation.cpp
        ../eco3d/src/precipitation.h
        ../eco3d/src/radiation.cpp
        ../eco3d/src/radiation.h
        ../eco3d/src/reach.cpp
        ../eco3d/src/reach.h
        ../eco3d/src/respiration.cpp
        ../eco3d/src/respiration.h
        ../eco3d/src/root.cpp
        ../eco3d/src/root.h
        ../eco3d/src/segment.cpp
        ../eco3d/src/segment.h
        ../eco3d/src/snow.cpp
        ../eco3d/src/snow.h
        ../eco3d/src/soil.cpp
        ../eco3d/src/soil.h
        ../eco3d/src/stem.cpp
        ../eco3d/src/stem.h
        ../eco3d/src/stream.cpp
        ../eco3d/src/stream.h
        ../eco3d/src/surface_runoff.cpp
        ../eco3d/src/surface_runoff.h
        ../eco3d/src/system.cpp
        ../eco3d/src/system.h
        ../eco3d/src/temperature.cpp
        ../eco3d/src/temperature.h
        ../eco3d/src/vegetation.cpp
        ../eco3d/src/vegetation.h)

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

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/bin)
install(TARGETS eco3d RUNTIME DESTINATION ${INSTALL_DIR})

find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_link_libraries(eco3d -std=c++11)
