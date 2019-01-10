#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/iterator.hpp>

#include "libpokerengine/pokerengine.hpp"

// overloaded method signatures
#include <string>

BOOST_PYTHON_MODULE(pypoker) {
	using namespace boost::python;
	// custom vector wrapper for list like results
	class_<CustomList>("CustomList")
		.def(vector_indexing_suite<CustomList>())
	;

	// exposing right method (overloaded with boost.python types)
	// NOTE: We define pointer to 'EV' method which signature is (boost::python::list, std::string, double)
	// NOTE: Left-Side pointer to the EV method of PokerEngine
	// NOTE: Right-Side adress of PokerEngine::EV with matching signature
	boost::python::list (PokerEngine::*EV)(list, std::string, double) = &PokerEngine::EV;
	class_<PokerEngine>("PokerEngine", init<>())
		.def("EV", EV)
		//.def("EV", &PokerEngine::EV)
	;
}

