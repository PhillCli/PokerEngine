list(APPEND sources
	pokerengine.cpp
	)

add_library(pokerengine ${sources})

# -fPIC required for python bidings
target_compile_options(pokerengine PRIVATE -fPIC -std=c++11)
target_link_libraries(pokerengine pokercore ${Boost_LIBRARIES})

