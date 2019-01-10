#include <string>
#include <vector>

// boost.Python
#include <boost/python.hpp>

#include "libpokercore/pokercore.h"

#ifndef POKERENGINE_H
#define POKERENGINE_H

typedef std::vector<double> CustomList;

class PokerEngine {
	public:
		PokerEngine();
		~PokerEngine();
		// 2hands & board
		CustomList EV(std::string hand1, std::string hand2, std::string board, double runs);
		std::vector<double> EV(std::vector<std::string>, std::string board, double runs);
		boost::python::list EV(boost::python::list hands, std::string board, double runs);
		
};


#endif
