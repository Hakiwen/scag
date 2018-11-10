//
// Created by jbm on 11/9/18.
//

#include "game.h"


game::game(const game& rhs) : traveler_strat(rhs.traveler_strat), ambusher_strat(rhs.ambusher_strat), game_environment(rhs.game_environment), outcome(rhs.outcome) {}

//game outcome element of non-negative reals
game::game(game&& rhs) noexcept : traveler_strat(std::move(rhs.traveler_strat)), ambusher_strat(std::move(rhs.ambusher_strat)), game_environment(std::move(rhs.game_environment)), outcome(rhs.outcome) {rhs.outcome = -1;}

game& game::operator= (const game& rhs)
{
	this->traveler_strat = rhs.traveler_strat;
	this->ambusher_strat = rhs.ambusher_strat;
	this->game_environment = rhs.game_environment;
	this->outcome = rhs.outcome;
	return *this;
}

game& game::operator= (game&& rhs) noexcept
{
	this->traveler_strat = std::move(rhs.traveler_strat);
	this->ambusher_strat = std::move(rhs.ambusher_strat);
	this->game_environment = std::move(rhs.game_environment);
	this->outcome = rhs.outcome;
	rhs.outcome = -1;
	return *this;
}

game::~game()
{

}
game::game(traveler_strategy traveler_strat_, ambusher_strategy ambusher_strat_) : traveler_strat(traveler_strat_), ambusher_strat(ambusher_strat_)
{
		this->evaluate();
}

double game::get_outcome() const
{
	return this->outcome;
}

void game::evaluate()
{

}