list(APPEND sources
	pokercore.cpp
	)

add_library(pokercore ${sources})

# -fPIC required for python bidings
target_compile_options(pokercore PRIVATE -fPIC -std=c++11)

