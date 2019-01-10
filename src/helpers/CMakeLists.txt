list(APPEND sources
	helpers.cpp
	)

add_library(helpers ${sources})

# -fPIC required for python bidings
target_compile_options(helpers PRIVATE -fPIC -std=c++11)

