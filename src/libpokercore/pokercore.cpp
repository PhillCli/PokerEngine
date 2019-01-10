#include <iostream>
#include <cassert>

#include <stdlib.h> //srand, rand

#include "arrays.h"
#include "libpokercore/pokercore.h"

/* Implementation taken from "Poker hand evaluator"
 * by Kevin L. Suffecool
 *		kevin@suffe.cool
 *
 */

// TODO: can be improved by ussing SSE instruction
// Fast RNG https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
void PokerCore::seed_rand(int seed) {
	
	g_seed = seed;

}

int PokerCore::fast_rand() {
	
	g_seed = (214013*g_seed+2531011);
	return (g_seed>>16)&0x7FFF;

}

// TODO(~Filip):
// could be rewritten using perfect hash function
// maping key <-> index of occurence in products array
// key is product value of primes, 
// which translates into unique 5 card combo coded by index
// could speed up by aprox. 50-60%
int PokerCore::find_it(int key) {
	int low = 0, high = 4887, mid;

	while (low <= high) {
		mid = (high + low) >> 1;      // divide by two
		if (key < products[mid])
			high = mid - 1;
		else if (key > products[mid])
			low = mid + 1;
		else
			return mid;
	}
	//fprintf(stderr, "ERROR:  no match found; key = %d\n", key);
	std::cerr << "ERROR: find_it: no match found. key = " << key;
	std::cerr << " Not allowed card combo!" << std::endl;
	return -1;
}

//
//   This routine initializes the deck.  A deck of cards is
//   simply an integer array of length 52 (no jokers).  This
//   array is populated with each card, using the following
//   scheme:
//
//   An integer is made up of four bytes.  The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card
//
void PokerCore::init_deck(int *deck) {
    
	int i, j, n = 0, suit = 0x8000;
	for (i = 0; i < 4; i++, suit >>= 1)
		for (j = 0; j < 13; j++, n++)
		deck[n] = primes[j] | (j << 8) | suit | (1 << (16+j));

}

// guess pointers are necessary, since we are exploiting
// bitwise operations on ints
void PokerCore::print_hand(int *hand, int n) {
	int i, r;
	char suit;
	std::string rank = "23456789TJQKA";

	for (i = 0; i < n; i++) {
		r = (*hand >> 8) & 0xF;
		if (*hand & 0x8000)
				suit = 'c';
		else if (*hand & 0x4000)
				suit = 'd';
		else if (*hand & 0x2000)
				suit = 'h';
		else
				suit = 's';

		//printf("%c%c ", rank[r], suit);
		std::cout << rank[r] << suit << " ";
		hand++;
  }
	// end with '\n'
	std::cout << std::endl;
}

// draw card from the deck and put it on specified place
void PokerCore::draw_card(int *deck, int rank, int suit, int place) {
	int tmp, position;
	int i, suit_hex = 0x8000;
	for (i=0; i<suit; i++) {suit_hex >>=1;}
	position = find_card(rank, suit_hex, deck);
	tmp = deck[place];
	//printf("Card at position: %d\n", place);
	//print_hand(&deck[place], 1);
	//printf("\n");
	//printf("Card at position: %d\n", position);
	//print_hand(&deck[position], 1);
	//printf("\n");
	deck[place] = deck[position];
	deck[position]=tmp;
}

// TODO: Can be improved by LUT (card -> array posisiton), cause initiate deck same every time
// It will be important in range vs range calculations (multiple re-drawing for each player)
void PokerCore::draw_card(int *deck, int card, int place) {
	auto position = find_card(card, deck);
	auto tmp = deck[place];
	deck[place] = deck[position];
	deck[position] = tmp;
}

int PokerCore::find_card(int rank, int suit, int *deck)
{
    int i, c;

    for (i = 0; i < 52; i++)
    {
        c = deck[i];
        if ((c & suit) && (RANK(c) == rank))
            return i;
    }
    return -1;
}

int PokerCore::find_card(int card, int *deck)
{
    int i, c;

    for (i = 0; i < 52; i++)
    {
        c = deck[i];
        if (c == card)
            return i;
    }
    return -1;
}

int PokerCore::card2int(std::string card) {
	assert (card.length() == 2);
	
	std::string rank_str = "23456789TJQKA";
	std::string suit_str = "cdhs";

	int card_int = 0;
	auto card_rank = card[0];
	auto card_suit = card[1];
	auto rank = 0;
	auto suit = 0;

	auto suit_hex = 0x8000;

	// translate rank_str into rank
	for (char &c: rank_str) { if (c == card_rank) { break; } rank+=1; }
	// translate suit into hex_mask
	for (char &c: suit_str) { if (c == card_suit) { break; } suit+=1; suit_hex>>=1; }

	//std::cout << "[DEBUG]: " << card_rank << " is " << rank << std::endl; 
	//std::cout << "[DEBUG]: " << card_suit << " is " << suit << std::endl; 
	// constructing int representation
	card_int = primes[rank] | (rank << 8) | suit_hex | (1 << (16+rank));	
	return card_int;
}

void PokerCore::shuffle_deck(int *deck, int pos, int num) {
// shuffles only cards that are used in game
// deck is assummed to have at least num cards
// num starting offset of the shuffling
// currently it will shuffle last 5 cards in the deck ending at
// card deck[num];
	num -= 1;
	for (int i = pos; i > pos-num; i--) { 
		// we only need to draw 5 cards for the board (at max)
		int n = fast_rand() % (i + 1); // formula for ranged rand()
		// swap the cards
		auto tmp = deck[i];
		deck[i] = deck[n];
		deck[n] = tmp;
	}
}

short PokerCore::eval_5cards(int c1, int c2, int c3, int c4, int c5) {
	int q;
	short s;

	q = (c1 | c2 | c3 | c4 | c5) >> 16;

	// check for Flushes and StraightFlushes
	if (c1 & c2 & c3 & c4 & c5 & 0xF000)
	return flushes[q];

	// check for Straights and HighCard hands
	s = unique5[q];
	if (s)  return s;

	// let's do it the hard way
	q = (c1&0xFF) * (c2&0xFF) * (c3&0xFF) * (c4&0xFF) * (c5&0xFF);
	q = find_it(q);

	return values[q];
}

short PokerCore::eval_5hand(int *hand) {
	int c1, c2, c3, c4, c5;

	c1 = *hand++;
	c2 = *hand++;
	c3 = *hand++;
	c4 = *hand++;
	c5 = *hand;

	return eval_5cards(c1,c2,c3,c4,c5);
}

short PokerCore::eval_7hand(int *hand) {
	int i, j, q, best = 9999, subhand[5];

	for (i = 0; i < 21; i++)
	{
			for (j = 0; j < 5; j++)
					subhand[j] = hand[ perm7[i][j] ];
			q = eval_5hand(subhand);
			if (q < best)
					best = q;
	}
  return best;
}

