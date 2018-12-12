//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_GAME_MAP_H
#define SCAG_GAME_MAP_H

#include "square_obstacle.h"
#include "utils.h"


#define MIN_BOX_DIM 0.001
#define OBSTACLE_FILL 0.3
#define AMBUSH_RADIUS 0.13
#define NUM_TARGETS 4

struct edge{
	point head;
	point tail;

	edge(point head, point tail);
};
struct game_map {

	int grid_points;
	std::vector<square_obstacle> obstacles;
	std::vector<point> grid;
	std::vector<edge> edges;
	int * connectivity_matrix;
	int * D_matrix;
	int * distance_matrix;
	int num_source_nodes;
	int num_sink_nodes;
	std::vector<int> adjacency_matrix;
	std::vector<std::vector<int> > adjacency_matrix_powers;
	std::vector<double> solution;
	std::vector<double> searcher_paths;
	std::vector<double> ambush;
	std::vector<int> target_locations;
	int chosen_target_index;

	double outcome;


	game_map() = default;
	//create map with list of squares, width w_, and height h_
	game_map(int grid_points_, std::initializer_list<square_obstacle> obstacles_);
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
	std::vector<std::vector<int> > generate_adjacency_powers();
	int distance_between_vertices(int i, int j);
	int * generate_distance_matrix();
	std::vector<double> solve_game_traveler();
	std::vector<int> solve_game_hider();
	std::vector<double>solve_game_searcher();
	std::vector<double> solve_game_ambusher();
	double game_outcome();
};

#endif //SCAG_GAME_MAP_H
