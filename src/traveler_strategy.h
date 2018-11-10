//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_TRAVELER_STRATEGY_H
#define SCAG_TRAVELER_STRATEGY_H
#include "utils.h"

class traveler_strategy {
private:

public:
	traveler_strategy() = default;
	traveler_strategy(const traveler_strategy&);
	traveler_strategy(traveler_strategy&&) noexcept;
	traveler_strategy& operator=(const traveler_strategy&);
	traveler_strategy& operator=(traveler_strategy&&) noexcept;
	~traveler_strategy();
};


#endif //SCAG_TRAVELER_STRATEGY_H
