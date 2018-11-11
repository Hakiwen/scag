//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_GAME_H
#define SCAG_GAME_H


#include "traveler_strategy.h"
#include "ambusher_strategy.h"
#include "game_map.h"

class game {
private:
	traveler_strategy traveler_strat;
	ambusher_strategy ambusher_strat;
	game_map game_environment;
	double outcome;
public:
	game() = default;
	game(const game&);
	game(game&&) noexcept;
	game& operator=(const game&);
	game& operator=(game&&) noexcept;
	game(traveler_strategy, ambusher_strategy);

	double get_outcome() const;

	void evaluate();
};


#endif //SCAG_GAME_H
