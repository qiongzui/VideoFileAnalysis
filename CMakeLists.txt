cmake_minimum_required(VERSION 3.16)
project(VideoFileAnalysis)

set(CMAKE_CXX_STANDARD 14)

set(MUXER ${CMAKE_CURRENT_SOURCE_DIR}/muxer)
set(FM ${CMAKE_CURRENT_SOURCE_DIR}/frameworks)

include_directories(${MUXER})
include_directories(${FM})

aux_source_directory(${MUXER} MUXERFILE)
aux_source_directory(${FM} FMFILE)

add_executable(VideoFileAnalysis main.cpp ${MUXERFILE} ${FMFILE})