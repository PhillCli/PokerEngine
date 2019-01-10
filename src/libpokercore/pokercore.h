#ifndef POKERCORE_H
#define POKERCORE_H

#define	RANK(x)		((x >> 8) & 0xF)

static unsigned int g_seed; // seed for RNG

namespace PokerCore {

	// poker core routines
	int find_it(int key);
	void init_deck(int *deck);
	void print_hand(int *hand, int n);
	void draw_card(int *deck, int rank, int suit, int place);
	void draw_card(int *deck, int card, int place);
	void shuffle_deck(int *deck, int pos, int num);
	int find_card(int rank, int suit, int *deck);
	int find_card(int card, int *deck);
	int card2int(std::string card);

	short eval_5cards(int c1, int c2, int c3, int c4, int c5);
	short eval_5hand(int *hand);
	short eval_7hand(int *hand);

	// RNG routines
	int fast_rand();
	void seed_rand(int seed);

}

#endif
