//
// Created by jbm on 11/9/18.
//

#include "game_map.h"

game_map::game_map(std::initializer_list<square_obstacle> obstacles_, double w_, double h_) : width(w_),
																								height(h_),
																								obstacles(std::vector<square_obstacle>(obstacles_)){}
game_map::game_map(double n, double w_, double h_) : width(w_),
														height(h_) {
	obstacles = std::vector<square_obstacle>(n);
	std::default_random_engine gen;
	std::uniform_real_distribution<double> x_distribution(0.0, w_);
	std::uniform_real_distribution<double> y_disrtibution(0.0, h_);

	point anchor_point;

	for(int i = 0; i < n; i++){
		anchor_point = point(x_distribution(gen), y_disrtibution(gen));
		//the chosen omega should yield an expected obstacle density of OBSTACLE FILL, assuming MIN_BOX_DIM is low
		double omega = sqrt(3*OBSTACLE_FILL*w_*h_/n);
		double width_upper_bound = omega, height_upper_bound = omega;

		if(w_ - anchor_point.x + omega <= 0) {
			width_upper_bound = w_ - anchor_point.x;
		}
		if(anchor_point.y - omega <= 0) {
			height_upper_bound = anchor_point.y;
		}
		std::uniform_real_distribution<double>height_distribution(MIN_BOX_DIM, height_upper_bound);
		std::uniform_real_distribution<double>width_distribution(MIN_BOX_DIM, width_upper_bound);

		obstacles.emplace_back(square_obstacle(anchor_point, width_distribution(gen), height_distribution(gen)));
	}
}

game_map::game_map(const game_map & rhs) : obstacles(rhs.obstacles),
											width(rhs.width),
											height(rhs.height){}

game_map::game_map(game_map && rhs) noexcept : obstacles(std::move(rhs.obstacles)),
										width(rhs.width),
										height(rhs.height) {
	rhs.width = -1;
	rhs.height = -1;
}

game_map& game_map::operator=(const game_map & rhs) {
	this->obstacles = rhs.obstacles;
	this->width = rhs.width;
	this->height = rhs.height;

	return *this;
}

game_map& game_map::operator=(game_map && rhs) noexcept{
	this->obstacles = std::move(rhs.obstacles);
	this->width = rhs.width;
	this->height = rhs.height;

	rhs.width = -1;
	rhs.height = -1;

	return *this;
}