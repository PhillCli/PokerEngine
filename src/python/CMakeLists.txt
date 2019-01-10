list(APPEND sources
	pypoker.cpp
	)

add_library(pypoker SHARED ${sources})
target_compile_options(pypoker PRIVATE -std=c++11)
target_link_libraries(pypoker pokerengine pokercore ${Boost_LIBRARIES} ${PROJECT_NAME})

# Suppress prefix "lib" because Python does not allow this prefix
set_target_properties(pypoker PROPERTIES PREFIX "")

install(TARGETS pypoker DESTINATION lib)

install(FILES __init__.py
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES pypokerclient.py
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_EXECUTE GROUP_READ)


