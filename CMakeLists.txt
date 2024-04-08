cmake_minimum_required(VERSION 3.15.0)
project(ChessGame VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()
include_directories(src)
aux_source_directory(src DIR_SRCS)
add_executable(ChessGame ${DIR_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)