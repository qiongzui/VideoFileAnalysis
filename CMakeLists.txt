cmake_minimum_required(VERSION 3.16)
project(VideoFileAnalysis)

set(CMAKE_CXX_STANDARD 14)

set(3PARTY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(TEST ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(MUXER ${CMAKE_CURRENT_SOURCE_DIR}/muxer)
set(FM ${CMAKE_CURRENT_SOURCE_DIR}/frameworks)
set(DECODER ${CMAKE_CURRENT_SOURCE_DIR}/decoder)
set(PASER ${CMAKE_CURRENT_SOURCE_DIR}/paser)

include_directories(${3PARTY}/include)
include_directories(${TEST})
include_directories(${MUXER})
include_directories(${FM})
include_directories(${DECODER})
include_directories(${PASER})

aux_source_directory(${MUXER} MUXERFILE)
aux_source_directory(${TEST} TESTFILE)
aux_source_directory(${FM} FMFILE)
aux_source_directory(${DECODER} DECODERFILE)
aux_source_directory(${PASER} PASERFILE)

link_directories(${3PARTY}/lib/gtest)

add_executable(VideoFileAnalysis main.cpp
        ${MUXERFILE}
        ${FMFILE}
        ${DECODERFILE}
        ${PASERFILE}
        ${TESTFILE}
        )

target_link_libraries(VideoFileAnalysis
        gtest
        gtest_main)