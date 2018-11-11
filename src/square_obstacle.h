//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_SQUARE_OBSTACLE_H
#define SCAG_SQUARE_OBSTACLE_H

#include <sqltypes.h>
#include "point.h"
#include "utils.h"
class square_obstacle {
private:
	point top_left;
	point top_right;
	point bottom_left;
	point bottom_right;
	double width;
	double height;
	friend class game_map;
public:
	square_obstacle() = default;
	square_obstacle(point tl, point tr , point bl, point br); //define corners
	square_obstacle(std::initializer_list<point>);
	square_obstacle(point tl, double w , double h); //define top left, width, height
	square_obstacle(const square_obstacle &);
	square_obstacle(square_obstacle&&) noexcept;
	square_obstacle&operator=(const square_obstacle&);
	square_obstacle&operator=(square_obstacle&&)noexcept;
};


#endif //SCAG_SQUARE_OBSTACLE_H
