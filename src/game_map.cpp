//
// Created by jbm on 11/9/18.
//

#include "game_map.h"

game_map::game_map(std::initializer_list<square_obstacle> obstacles_, double w_, double h_) : width(w_),
																								height(h_),
																								obstacles(std::vector<square_obstacle>(obstacles_)){}
game_map::game_map(double n, double w_, double h_) : width(w_),
														height(h_) {
	this->obstacles = std::vector<square_obstacle>(n);
	std::default_random_engine gen;
	std::default_random_engine gen2;
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

		this->obstacles[i] = square_obstacle(anchor_point, width_distribution(gen), height_distribution(gen2));
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

bool game_map::is_point_in_X_free(point p) {
	bool ret = true;
	for(int i = 0; i < obstacles.size(); i++){
		ret = ret && !((p.x > obstacles[i].top_left.x) &&
						(p.x < obstacles[i].top_right.x) &&
				(p.y > obstacles[i].bottom_left.y) &&
				(p.y < obstacles[i].top_left.y));
	}
}

void game_map::draw() {
	glBegin(GL_QUADS);
	for(int i = 0; i < obstacles.size(); i++){
		glColor3f(0.0f,0.0f,0.0f);
		glVertex2d(obstacles[i].top_left.x/this->width , obstacles[i].top_left.y/this->height );
		glVertex2d(obstacles[i].bottom_left.x/this->width , obstacles[i].bottom_right.y/this->height );
		glVertex2d(obstacles[i].bottom_right.x/this->width , obstacles[i].bottom_right.y/this->height );
		glVertex2d(obstacles[i].top_right.x/this->width , obstacles[i].top_right.y/this->height );
	}
	glEnd();
}