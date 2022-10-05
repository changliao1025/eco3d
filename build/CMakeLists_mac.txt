cmake_minimum_required(VERSION 3.10)
project(eco3d CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB eco3d_srcs
        src/atmosphere.cpp
        src/atmosphere.h
        src/canopy.cpp
        src/canopy.h
        src/cascade.cpp
        src/cascade.h
        src/column.cpp
        src/column.h
        src/conversion.cpp
        src/conversion.h
        src/data.cpp
        src/data.h
        src/date.cpp
        src/date.h
        src/dimension.h
        src/eco3d.h
        src/eco3d_auxiliary.cpp
        src/eco3d_cascade.cpp
        src/eco3d_initialize.cpp
        src/eco3d_main.cpp
        src/eco3d_read.cpp
        src/eco3d_run.cpp
        src/eco3d_save.cpp
        src/eco3d_setup.cpp
        src/eco3d_stream.cpp
        src/evapotranspiration.cpp
        src/evapotranspiration.h
        src/geology.cpp
        src/geology.h
        src/glacier.cpp
        src/glacier.h
        src/global.cpp
        src/global.h
        src/groundwater.cpp
        src/groundwater.h
        src/hru.cpp
        src/hru.h
        src/infiltration.cpp
        src/infiltration.h
        src/interception.cpp
        src/interception.h
        src/lake.cpp
        src/lake.h
        src/land.cpp
        src/land.h
        src/landcover.cpp
        src/landcover.h
        src/litter.cpp
        src/litter.h
        src/litterfall.cpp
        src/litterfall.h
        src/main.cpp
        src/mathematics.cpp
        src/mathematics.h
        src/microbe.cpp
        src/microbe.h
        src/miscellaneous.cpp
        src/miscellaneous.h
        src/photosynthesis.cpp
        src/photosynthesis.h
        src/precipitation.cpp
        src/precipitation.h
        src/radiation.cpp
        src/radiation.h
        src/reach.cpp
        src/reach.h
        src/respiration.cpp
        src/respiration.h
        src/root.cpp
        src/root.h
        src/segment.cpp
        src/segment.h
        src/snow.cpp
        src/snow.h
        src/soil.cpp
        src/soil.h
        src/stem.cpp
        src/stem.h
        src/stream.cpp
        src/stream.h
        src/surface_runoff.cpp
        src/surface_runoff.h
        src/system.cpp
        src/system.h
        src/temperature.cpp
        src/temperature.h
        src/vegetation.cpp
        src/vegetation.h)

add_executable(eco3d ${eco3d_srcs})

set(default_build_type Debug)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type Debug)
endif()

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

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS eco3d RUNTIME DESTINATION ${INSTALL_DIR})

find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_link_libraries(eco3d -std=c++11)