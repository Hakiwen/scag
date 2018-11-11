//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_GAME_MAP_H
#define SCAG_GAME_MAP_H

#include "square_obstacle.h"

#define MIN_BOX_DIM 0.001
#define OBSTACLE_FILL 0.33

class game_map {
private:
	std::vector<square_obstacle> obstacles;
	double width;
	double height;
public:
	game_map() = default;
	//create map with list of squares, width w_, and height h_
	game_map(std::initializer_list<square_obstacle> obstacles_, double w_, double h_);
	//randomly generate n obstacles
	game_map(double n, double w_, double h_);
	game_map(const game_map&);
	game_map(game_map&&) noexcept;
	game_map&operator=(const game_map&);
	game_map&operator=(game_map&&) noexcept;

	void draw();
	bool is_point_in_X_free(point p);
};


#endif //SCAG_GAME_MAP_H
