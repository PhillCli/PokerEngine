#include <chrono>
#include <iostream>
#include <cassert>

#include <cstdlib>

#include <stdio.h>
#include <time.h> // time()

#include "libpokercore/pokercore.h"
#include "libpokerengine/pokerengine.hpp"

#include <string>
#include <limits> // numeric_limits<>::max()

using namespace PokerCore;

// TODO: should probably be moved to PokerCore
void fill_7cards(int *cards7, int *cards2, int *cards5) {
	cards7[0] = cards2[0];
	cards7[1] = cards2[1];
	cards7[2] = cards5[0];
	cards7[3] = cards5[1];
	cards7[4] = cards5[2];
	cards7[5] = cards5[3];
	cards7[6] = cards5[4];
}

PokerEngine::PokerEngine() {
}

PokerEngine::~PokerEngine() {
}

// NOTE: ACHTUNG
// Should be only used for converting compact values e.g. function returning small lists
// For containers of big datas,  you want to create np.array with vector data pointer
// so you can avoid unecessary memory copying.
template<class T>
boost::python::list std_vector_to_py_list(const std::vector<T>& v) {
	boost::python::list list;
	typename std::vector<T>::const_iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		list.append(*it);
	}
	return list;
}

// We explicitly implement Python Wrapper as specialization
boost::python::list PokerEngine::EV(boost::python::list hands, std::string board, double runs=1e5) {
	auto hands_py_len = boost::python::len(hands);

	// need to be dynamic, 1-8 hands can be passed at runtime
	std::vector<std::string> hands_cpp(hands_py_len);

	// extracting hand arguments into cpp container
	for (auto i = 0; i < hands_py_len; i++) {
		hands_cpp[i] = boost::python::extract<std::string>(hands[i]);
		//std::cout << "[DEBUG] " << i << " " << hands_cpp[i] << std::endl;
	}

	// calling c++ method
	auto ev_results = PokerEngine::EV(hands_cpp, board, runs);

	// constructing return Py_List object (via boost)
	auto ev_results_py = std_vector_to_py_list(ev_results);
	return ev_results_py;
}

// general version, multiple players EV's
// TODO: Can be improved by explicitly unrolling players loop
// TODO: Input sanitization, No players etc.
std::vector<double> PokerEngine::EV(std::vector<std::string> hands, std::string board_str, double runs) {
	size_t HAND_SIZE = 2;
	size_t DECK_SIZE = 52;
	size_t COMMON_SIZE = 7;
	size_t BOARD_SIZE = 5;
	size_t NO_PLAYERS = hands.size();
	assert(NO_PLAYERS > 1);

	int deck[DECK_SIZE], common_cards[COMMON_SIZE];
	//int board[BOARD_SIZE];

	// initialize accumulators explicitly to 0
	int tmp_values[NO_PLAYERS] = {0};
	double w_wins[NO_PLAYERS] = {0.0f};
	//double comp_wins[NO_PLAYERS] = {0.0f}; // compensation for kahan float sum

	// place where board cards starts (reverse order)
	int board_offset = DECK_SIZE - NO_PLAYERS*HAND_SIZE - BOARD_SIZE;

	// RNG seed
	seed_rand(time(NULL));

	// initialize deck
	// produces same order of cards every time
	init_deck(deck);

	// parsing str hands to int representation
	for (size_t player_idx = 0; player_idx < NO_PLAYERS; player_idx++) {
		// TODO: add place holder for players e.g. AxY -> any ace with any other card etc.
		auto hand = hands[player_idx];
		//std::cout << "Player[" << player_idx << "]: " << hand << std::endl;

		// drawing hands
		// Individual cards are represented as 32 bit integers
		// deck layout is following:
		// 0 1 ...																		49   50		51   52
		// X X X X X ... B5 B4 B3 B2 B1 PK_2 PK_1 ... P2_2 P2_1 P1_2 P1_1
		// P1_1 is 1st card of player no 1 etc.
		// In this layout we put fixed card at the end, so we can easily randomly draw 'variable' cards
		// As we can see only K*2 + 5 cards are fixed (K - no of players, no dead cards, no placeholders)
		// Therefore we draw K*2 cards once
		// Then for each MC run we have to draw only 5 cards for the board
		for (size_t card=0; card < HAND_SIZE; card++) {
			auto card_int = card2int(hand.substr(card*HAND_SIZE, 2));
			draw_card(deck, card_int, (DECK_SIZE-1)-(player_idx*HAND_SIZE)-card);
		}
	}

	// MC probing of the board
	for (auto run=0; run < runs; run++) {
		// offset where players hands begins
		int players_offset = DECK_SIZE - NO_PLAYERS*HAND_SIZE - 1;
		shuffle_deck(deck, players_offset, BOARD_SIZE + 1);

		// TODO: check if unrolling for each case (NO PLAYERS) will improve performance
		// TODO: probably template with <NO_PLAYERS> will be enough

		// get best 7card combo for (given player, MC board)
		// better combo, lower value
		int best_value = std::numeric_limits<int>::max();

		for (size_t player_idx = 0; player_idx < NO_PLAYERS; player_idx++) {
			// TODO: define macros for this in PokerCore
			int hand_offset = DECK_SIZE - (player_idx + 1)*HAND_SIZE;

			// fill common cards
			fill_7cards(common_cards, &deck[hand_offset], &deck[board_offset]);

			// evaluate value (lower better)
			auto player_value = eval_7hand(common_cards);

			// update best hand
			if (player_value < best_value) {
				best_value = player_value;
			}

			// cache the results
			tmp_values[player_idx] = player_value;

		}

		double no_winners = 0;
		std::vector<int> tmp_draw;
		tmp_draw.reserve(NO_PLAYERS);

		// indentify number of draws, and locate players that are tied
		for (size_t player_idx = 0; player_idx < NO_PLAYERS; player_idx++) {
			if (tmp_values[player_idx] == best_value) {
				tmp_draw.push_back(player_idx);
				no_winners++;
			}
		}

		// weight for the winners
		// i.e. if one player wins he get full ev for that run
		double weight = 1.0f/no_winners;

		// update accumulators
		for (auto const& player_idx: tmp_draw) {
			// kahan float summation
			/*
			double run_comp = weight - comp_wins[player_idx];
			double tmp = w_wins[player_idx] + run_comp;
			comp_wins[player_idx] = (tmp - w_wins[player_idx]) - run_comp;
			w_wins[player_idx] = tmp;
			*/
			w_wins[player_idx] += weight;
		}

	}

	std::vector<double> result(NO_PLAYERS+1);
	for (size_t player_idx = 0; player_idx < NO_PLAYERS; player_idx++) {
		//std::cout << "Player[" << player_idx << "]: " << w_wins[player_idx] << " ";
		result[player_idx] = w_wins[player_idx]/runs;
	}

	// last element is number of runs
	result[NO_PLAYERS] = runs;

	return result;
}

