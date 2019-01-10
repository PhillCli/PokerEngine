cmake_minimum_required(VERSION 2.8.3)
project(EvEngine)

set (EvEngine_VERSION_MAJOR 0)
set (EvEngine_VERSION_MINOR 1)
set (EV_DATA ${PROJECT_SOURCE_DIR}/data)

include_directories(src)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)

