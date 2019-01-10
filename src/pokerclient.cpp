#include "libpokerengine/pokerengine.hpp"
#include <vector>
#include <string>
#include <iostream>

int main() {

	std::cout << "[TEST] Checking C++ API ..." << std::endl;
	PokerEngine engine = PokerEngine();
	std::vector<std::string> hands = {"Js6s", "AcTs"};
	std::string board = "";
	auto results = engine.EV(hands, board, 1e5);

	size_t idx = 0;
	for (auto const& hand: hands) {
		std::cout << hand << " " << 100*results[idx] << std::endl;
		idx++;
	}

	return EXIT_SUCCESS;
}
