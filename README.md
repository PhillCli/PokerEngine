### Overview
Texas Hold'em EV engine with C++/Python's bindings.
Only Monte Carlo probing supported (so far).
Core algorithm taken from "Poker hand evaluator" by Kevin L. Suffecool http://suffe.cool/poker/evaluator.html

### Building on Ubuntu
1. Install required dependencies
	1. [GCC]
	```
	$ sudo apt-get install build-essential g++ python-dev
	```
	2. [Boost.Python & Boost.test]
	```
	$ sudo apt-get install libboost-python-dev libboost-test-dev
	```
	3. [CMAKE]
	```
	$ sudo apt-get install cmake
	```

2. Building
	```
	$ mkdir build && cd build
	$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="your/install/path" ..
	$ make -j2 && make test ARGS=-j2
	$ make install
	```

### Examples
Compiled example binaries can be found in installation dir under /bin also see source at
src/pokerclient.cpp src/python/pypokerclient.py.

Add path to install dir's lib directory to your PYTHONPATH in order for script in /bin to work properly
```
$ export PYTHONPATH=${PYTHONPATH}:/YOUR/INSTALL/PATH/lib/
```

### TODO
0. ASAP:
  1. Travis config
  2. Benchmark tool
1. PokerEngine
  1. range based ev calculations
  2. dead card support
  3. board tests
  4. (exhaustive routines?)
2. PokerCore
  1. replace binary search tree search with new LUT (perfect hash function)
  2. SSE RNG routines (4 random numbers at one); inline ASM code
3. OpenMP support

