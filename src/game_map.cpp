//
// Created by jbm on 11/9/18.
//

#include "game_map.h"

edge::edge(point head_, point tail_) : head(head_), tail(tail_) {}

game_map::game_map(double n, int grid_points_) : grid_points(grid_points_){
	this->obstacles = std::vector<square_obstacle>(n);
	std::chrono::high_resolution_clock::time_point seed_beg = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point seed_end = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration seed_dur = seed_end - seed_beg;
	std::default_random_engine gen(seed_dur.count());
	std::default_random_engine gen2(seed_dur.count());
	std::uniform_real_distribution<double> x_distribution(0.0, 1);
	std::uniform_real_distribution<double> y_disrtibution(0.0, 1);

	point anchor_point;

	for(int i = 0; i < n; i++){
		anchor_point = point(x_distribution(gen), y_disrtibution(gen));
		//the chosen omega should yield an expected obstacle density of OBSTACLE FILL, assuming MIN_BOX_DIM is low
		double omega = sqrt(3*OBSTACLE_FILL/n);
		double width_upper_bound = omega, height_upper_bound = omega;

		if(1 - anchor_point.x + omega <= 0) {
			width_upper_bound = 1 - anchor_point.x;
		}
		if(anchor_point.y - omega <= 0) {
			height_upper_bound = anchor_point.y;
		}
		std::uniform_real_distribution<double>height_distribution(MIN_BOX_DIM, height_upper_bound);
		std::uniform_real_distribution<double>width_distribution(MIN_BOX_DIM, width_upper_bound);

		this->obstacles[i] = square_obstacle(anchor_point, width_distribution(gen), height_distribution(gen2));
	}
	this->grid = generate_grid();
	this->edges = generate_edges();
	this->connectivity_matrix = generate_connectivity_matrix();
	this->D_matrix = generate_D_matrix();

	for(int i = 0; i < edges.size()*grid.size(); i ++)
	{
		if(!(i % edges.size()))
			std::cout << std::endl;
		std::cout << D_matrix[i] << ", ";
	}
}

game_map::game_map(const game_map & rhs) : obstacles(rhs.obstacles) {}

game_map::game_map(game_map && rhs) noexcept : obstacles(std::move(rhs.obstacles)) {


}

game_map& game_map::operator=(const game_map & rhs) {
	this->obstacles = rhs.obstacles;



	return *this;
}

game_map& game_map::operator=(game_map && rhs) noexcept{
	this->obstacles = std::move(rhs.obstacles);
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
	return ret;
}

void game_map::draw() {
	glBegin(GL_QUADS);
	for(int i = 0; i < obstacles.size(); i++){
		glColor3f(0.0f,0.0f,0.0f);
		glVertex2d(obstacles[i].top_left.x, obstacles[i].top_left.y);
		glVertex2d(obstacles[i].bottom_left.x, obstacles[i].bottom_right.y);
		glVertex2d(obstacles[i].bottom_right.x, obstacles[i].bottom_right.y);
		glVertex2d(obstacles[i].top_right.x, obstacles[i].top_right.y);
	}
	glEnd();

	glBegin(GL_POINTS);

//	double point_size = 0.001;
	for(int i = 0; i < grid.size(); i++){
		glColor3f(0.0f,0.0f,0.0f);
		if(is_point_in_X_free(grid[i])) {
			glVertex2d(grid[i].x,grid[i].y);
		}
//			glVertex2d(grid[i][j].x/this->width + point_size, grid[i][j].y/this->height + point_size);
//			glVertex2d(grid[i][j].x/this->width - point_size, grid[i][j].y/this->height + point_size);
//			glVertex2d(grid[i][j].x/this->width - point_size, grid[i][j].y/this->height - point_size);
//			glVertex2d(grid[i][j].x/this->width + point_size, grid[i][j].y/this->height - point_size);
	}
	glEnd();

	glBegin(GL_LINES);
	for(int i = 0; i < edges.size(); i++){
		glColor3d(1.0f,0.0f,0.0f);
//		glColor3f(1.0f,1.0f,1.0f);
		glVertex2d(edges[i].head.x, edges[i].head.y);
		glVertex2d(edges[i].tail.x, edges[i].tail.y);
	}
	glEnd();


}

bool game_map::is_edge_in_X_free(point p1, point p2) {
	if(!(is_point_in_X_free(p1) && is_point_in_X_free(p2)))
		return false;

	bool ret = true;
	for(int i = 0; i < obstacles.size() && ret; i++)
	{
		ret = ret && !(do_line_segments_cross(p1, p2, obstacles[i].top_right, obstacles[i].top_left));
		ret = ret && !(do_line_segments_cross(p1, p2, obstacles[i].top_right, obstacles[i].bottom_right));
		ret = ret && !(do_line_segments_cross(p1, p2, obstacles[i].top_left, obstacles[i].bottom_left));
		ret = ret && !(do_line_segments_cross(p1, p2, obstacles[i].bottom_right, obstacles[i].bottom_left));
	}
	return ret;
}

bool game_map::do_line_segments_cross(point p1, point p2, point q1, point q2){
	point vp = point(p2.x - p1.x, p2.y - p1.y);
	point vq = point(q2.x - q1.x, q2.y - q1.y);
	double det = vq.x*vp.y - vq.y*vp.x;
	if(det) {
		double s = (1 / det) * ((p1.x - q1.x) * vp.y - (p1.y - q1.y) * vp.x);
		double t = (-1 / det) * ((p1.y - q1.y) * vq.x - (p1.x - q1.x) * vq.y);
		if((s > 0) && (s < 1) && (t > 0) && (t < 1))
			return true;
	}else{
		return false;
	}
}

std::vector<point> game_map::generate_grid(){
	int row_size = sqrt(grid_points);
	int column_size = sqrt(grid_points);
	double spacing = 1.0/(row_size - 1);
	std::vector<point> ret((row_size)*(column_size));

	for(int i = 0; i < (column_size)*(row_size); i++){
		ret[i] = point((i % row_size)*spacing, (i/row_size)*spacing);
	}
	return ret;
}

std::vector<edge> game_map::generate_edges(){
	std::vector<edge> ret;
	for(int i = 0; i < grid.size(); i++){
		for(int j = i + 1; j < grid.size(); j++){
			double norm = grid[i]^grid[j];
			if((norm <= sqrt(2.2)/(sqrt(grid_points) - 1)) && is_edge_in_X_free(grid[i], grid[j]))
				ret.push_back(edge(grid[i], grid[j]));
		}
	}
	return ret;
}

int* game_map::generate_connectivity_matrix() {
	int * ret = (int * ) malloc((edges.size()*grid.size())*sizeof(int));
	for(int i = 0; i < grid.size() ; i++){
		for(int j = 0;j < edges.size() ; j++){
			if(grid[i] == edges[j].head)
				ret[i*edges.size() + j] = - 1;
			else if(grid[i] == edges[j].tail)
				ret[i*edges.size() + j] = 1;
			else
				ret[i*edges.size() + j] = 0;
		}

	}
	return ret;
}
//half-assed formulation, currently just checking if end points are in minimum radius
int * game_map::generate_D_matrix(){
	int * ret = (int *) malloc((edges.size()*grid.size())*sizeof(int));
	for(int i = 0; i < grid.size(); i++) {
		for(int j = 0; j < edges.size(); j++){
			if(((edges[j].head^grid[i]) < AMBUSH_RADIUS) || ((edges[i].tail^grid[i]) < AMBUSH_RADIUS))
				ret[i*edges.size() + j] = 1;
			else
				ret[i*edges.size() + j] = 0;
		}
	}
	return ret;
}