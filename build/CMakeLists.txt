cmake_minimum_required(VERSION 3.10)
project(eco3d CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(eco3d_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/atmosphere.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/atmosphere.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/canopy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/canopy.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/cascade.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/cascade.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/column.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/column.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/conversion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/conversion.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/data.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/data.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/date.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/date.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/dimension.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_auxiliary.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_cascade.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_initialize.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_read.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_run.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_save.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_setup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/eco3d_stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/evapotranspiration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/evapotranspiration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/geology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/geology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/glacier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/glacier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/global.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/global.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/groundwater.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/groundwater.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/hru.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/hru.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/infiltration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/infiltration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/interception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/interception.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/lake.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/lake.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/land.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/land.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/landcover.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/landcover.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/litter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/litter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/litterfall.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/litterfall.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/mathematics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/mathematics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/microbe.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/microbe.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/miscellaneous.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/miscellaneous.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/photosynthesis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/photosynthesis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/precipitation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/precipitation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/radiation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/radiation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/reach.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/reach.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/respiration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/respiration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/root.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/root.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/segment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/segment.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/snow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/snow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/soil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/stem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/stem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/stream.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/surface_runoff.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/surface_runoff.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/system.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/system.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/temperature.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/temperature.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/vegetation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../eco3d/src/vegetation.h)

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
