//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_GAME_MAP_H
#define SCAG_GAME_MAP_H

#include "square_obstacle.h"
#include "utils.h"


#define MIN_BOX_DIM 0.001
#define OBSTACLE_FILL 0.33
#define AMBUSH_RADIUS 0.01
struct edge{
	point head;
	point tail;

	edge(point head, point tail);
};
class game_map {
private:
	int grid_points;
	std::vector<square_obstacle> obstacles;
	std::vector<point> grid;
	std::vector<edge> edges;
	int * connectivity_matrix;
	int * D_matrix;
public:
	game_map() = default;
	//create map with list of squares, width w_, and height h_
	game_map(std::initializer_list<square_obstacle> obstacles_);
	//randomly generate n obstacles
	game_map(double n, int grid_points_);
	game_map(const game_map&);
	game_map(game_map&&) noexcept;
	game_map&operator=(const game_map&);
	game_map&operator=(game_map&&) noexcept;

	void draw();
	bool is_point_in_X_free(point p);
	bool is_edge_in_X_free(point p1, point p2);
	bool do_line_segments_cross(point p1, point p2, point q1, point q2);
	std::vector<point> generate_grid();
	std::vector<edge> generate_edges();
	int * generate_connectivity_matrix();
	int * generate_D_matrix();
};

#endif //SCAG_GAME_MAP_H
