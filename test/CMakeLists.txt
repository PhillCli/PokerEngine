enable_testing()

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

file(GLOB_RECURSE TEST_CONF "./*.cpp.in")

foreach(testConf ${TEST_CONF})
    get_filename_component(testName ${testConf} NAME_WE)
    get_filename_component(testDir  ${testConf} DIRECTORY)

    set(testSrc "${testDir}/${testName}.cpp")

    configure_file(${testConf} ${testSrc})
    add_executable(${testName} ${testSrc})

    target_compile_options(${testName} PRIVATE -std=c++11)
    target_link_libraries(${testName} core helpers ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} )

    add_test(NAME ${testName} COMMAND $<TARGET_FILE:${testName}>)

endforeach(testConf)

