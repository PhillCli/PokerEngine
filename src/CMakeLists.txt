cmake_minimum_required(VERSION 2.8.3)
project(core C CXX)

FIND_PACKAGE(PythonInterp REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(Boost REQUIRED COMPONENTS python) #unit_test_framework)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

add_library(core SHARED "")

file(GLOB_RECURSE project_headers
    "${PROJECT_SOURCE_DIR}/*/*.h*")

set_target_properties(core PROPERTIES
    PUBLIC_HEADER "${project_headers}")

include_directories("${PROJECT_SOURCE_DIR}/libpokercore")
add_subdirectory(libpokercore)

include_directories("${PROJECT_SOURCE_DIR}/libpokerengine")
add_subdirectory(libpokerengine)

# python bindings
include_directories("${PROJECT_SOURCE_DIR}/pypoker")
add_subdirectory(python)

target_link_libraries(core pokercore pokerengine)
install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


# example binary
add_executable(pokerapp pokerclient.cpp)
target_compile_options(pokerapp PRIVATE -std=c++11)
target_link_libraries(pokerapp core ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS pokerapp
    DESTINATION bin)

# helper functionalities
project(helper CXX)
add_library(helper SHARED "")
include_directories("${PROJECT_SOURCE_DIR}/helpers")
add_subdirectory(helpers)

target_link_libraries(helper helpers)
install(TARGETS helper
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

