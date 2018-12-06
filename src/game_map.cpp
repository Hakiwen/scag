//
// Created by jbm on 11/9/18.
//

#include "game_map.h"

edge::edge(point head_, point tail_) : head(head_), tail(tail_) {}

game_map::game_map(double n, int grid_points_) : grid_points(((int)sqrt(grid_points_))*((int)sqrt(grid_points_))) {
	this->obstacles = std::vector<square_obstacle>(n);
	std::chrono::high_resolution_clock::time_point seed_beg = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point seed_end = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration seed_dur = seed_end - seed_beg;
	std::default_random_engine gen(seed_dur.count());
	std::default_random_engine gen2(seed_dur.count());
	std::uniform_real_distribution<double> x_distribution(0.0, 1);
	std::uniform_real_distribution<double> y_disrtibution(0.0, 1);

	point anchor_point;

	for (int i = 0; i < n; i++) {
		anchor_point = point(x_distribution(gen), y_disrtibution(gen));
		//the chosen omega should yield an expected obstacle density of OBSTACLE FILL, assuming MIN_BOX_DIM is low
		double omega = sqrt(3 * OBSTACLE_FILL / n);
		double width_upper_bound = omega, height_upper_bound = omega;

		if (1 - anchor_point.x + omega <= 0) {
			width_upper_bound = 1 - anchor_point.x;
		}
		if (anchor_point.y - omega <= 0) {
			height_upper_bound = anchor_point.y;
		}
		std::uniform_real_distribution<double> height_distribution(MIN_BOX_DIM, height_upper_bound);
		std::uniform_real_distribution<double> width_distribution(MIN_BOX_DIM, width_upper_bound);

		this->obstacles[i] = square_obstacle(anchor_point, width_distribution(gen), height_distribution(gen2));
	}
	this->grid = generate_grid();
	this->edges = generate_edges();
	this->connectivity_matrix = generate_connectivity_matrix();
	this->D_matrix = generate_D_matrix();

	this->adjacency_matrix_powers = generate_adjacency_powers();
	this->distance_matrix = generate_distance_matrix();
	this->solution = solve_game_traveler();
//	this->ambush = solve_game_ambusher();
	this->target_locations = solve_game_hider();
//	this->outcome = game_outcome();
	std::cout << this->outcome << std::endl;
//	for(int i = 0; i < grid.size()*grid.size(); i ++)
//	{
//		if(!(i % grid.size()))
//			std::cout << std::endl;
//		std::cout << distance_matrix[i] << ", ";

//	}
//	for(int i = 0; i < grid.size()*grid.size(); i ++)
//	{
//		if(!(i % grid.size()))
//			std::cout << std::endl;
//		std::cout << adjacency_matrix[i] << ", ";
//	}
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

void draw_circle(point p, double r, double alpha)
{
	int n_segments = 100;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(1.0,0.0,0.0);
	glVertex2d(2*p.x -1, 2*p.y -1);
	for(int i = 0; i < n_segments; i++){
		double theta = (2.0f*M_PI*i)/n_segments;
		double x = r*cosf(theta);
		double y = r*sinf(theta);

		glVertex2d(x + 2*p.x - 1, y - 1 + 2*p.y);
	}
	glEnd();
}

void game_map::draw() {
	glBegin(GL_QUADS);
	for(int i = 0; i < obstacles.size(); i++){
		glColor3f(0.0f,0.0f,0.0f);
		glVertex2d(2*obstacles[i].top_left.x - 1.0, 2*obstacles[i].top_left.y - 1.0);
		glVertex2d(2*obstacles[i].bottom_left.x - 1.0, 2*obstacles[i].bottom_right.y - 1.0);
		glVertex2d(2*obstacles[i].bottom_right.x - 1.0, 2*obstacles[i].bottom_right.y - 1.0);
		glVertex2d(2*obstacles[i].top_right.x - 1.0, 2*obstacles[i].top_right.y - 1.0);
	}
	glEnd();

//	glBegin(GL_POINTS);

//	double point_size = 0.001;
//	for(int i = 0; i < grid.size(); i++){
//		glColor3f(0.0f,0.0f,0.0f);
//		if(is_point_in_X_free(grid[i])) {
//			glVertex2d(2*grid[i].x - 1.0 ,2*grid[i].y - 1.0);
//		}
//			glVertex2d(grid[i][j].x/this->width + point_size, grid[i][j].y/this->height + point_size);
//			glVertex2d(grid[i][j].x/this->width - point_size, grid[i][j].y/this->height + point_size);
//			glVertex2d(grid[i][j].x/this->width - point_size, grid[i][j].y/this->height - point_size);
//			glVertex2d(grid[i][j].x/this->width + point_size, grid[i][j].y/this->height - point_size);
//	}
//	glEnd();



	glBegin(GL_LINES);
	for(int i = 0; i < edges.size(); i++){
		if(solution[i] > 0) {
			glColor3d(0.0f, 0.0f, 1.0f);

		}
		else {
			glColor3d(0.9f, 0.9f, 0.9f);

		}
// glColor3f(1.0f,1.0f,1.0f);
		glVertex2d(2*edges[i].head.x - 1.0, 2*edges[i].head.y - 1.0);
		glVertex2d(2*edges[i].tail.x - 1.0, 2*edges[i].tail.y - 1.0);
	}
	glEnd();

	for(int i = 0;i < grid.size(); i++){
		if(target_locations[i])
			draw_circle(grid[i],AMBUSH_RADIUS, 1.0f);
	}

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
		ret[i] = point((i / row_size)*spacing, (i % row_size)*spacing);
	}
	return ret;
}

std::vector<edge> game_map::generate_edges(){
	std::vector<edge> ret;
	ret.reserve(grid_points*grid_points);
	adjacency_matrix = std::vector<int>(grid.size()*grid.size(), 0);

	for(int i = 0; i < grid.size(); i++){
		for(int j = i + 1; j < grid.size(); j++){
			double norm = grid[i]^grid[j];
			if((norm <= sqrt(2.5)/(sqrt(grid_points) - 1)) && is_edge_in_X_free(grid[i], grid[j])) {
				ret.push_back(edge(grid[i], grid[j]));
				adjacency_matrix[i*grid.size() + j] = 1;
				adjacency_matrix[j*grid.size() + i] = 1;
			}
		}
	}
	return ret;
}

int* game_map::generate_connectivity_matrix() {
	int * ret = (int * ) malloc(edges.size()*grid.size()*sizeof(int));
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



std::string itos(int i) {std::stringstream s; s << i; return s.str(); }
std::vector<double> game_map::solve_game_traveler(){
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		GRBVar *vars = new GRBVar[edges.size()];
		for (int i = 0; i < edges.size(); i++) {
			std::string name = "p_" + itos(i);
			vars[i] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "p" + itos(i));
		}
		GRBVar z = model.addVar(0.0, 10000.0, 1.0, GRB_CONTINUOUS, "z");
		GRBLinExpr obj_function(z);
		model.setObjective(obj_function);
		for (int i = sqrt(grid_points); i < grid.size() - sqrt(grid_points); i++) {
			GRBLinExpr flow_const = 0;
			for (int j = 0; j < edges.size(); j++) {
				flow_const += connectivity_matrix[i * edges.size() + j] * vars[j];
			}
			model.addConstr(flow_const == 0.0, "r_" + itos(i));
		}

		GRBLinExpr source_const = 0;
		for (int i = 0; i < sqrt(grid_points); i++) {
			for (int j = 0; j < edges.size(); j++) {
				source_const += connectivity_matrix[i * edges.size() + j] * vars[j];
			}
		}
		model.addConstr(source_const == -1.0, "s");

		GRBLinExpr sink_const = 0;
		for (int i = grid.size() - sqrt(grid_points); i < grid.size(); i++) {
			for (int j = 0; j < edges.size(); j++) {
				sink_const += connectivity_matrix[i * edges.size() + j] * vars[j];
			}
		}
		model.addConstr(sink_const == 1.0, "t");


		for (int i = 0; i < grid.size(); i++) {
			GRBLinExpr slack_const = 0;
			for (int j = 0; j < edges.size(); j++) {
				slack_const += D_matrix[i * edges.size() + j] * vars[j] - z;
			}
			model.addConstr(slack_const <= 0.0, "z_" + itos(i));
		}

		model.optimize();

		std::vector<double> ret(edges.size());
		for (int i = 0; i < edges.size(); i++) {
			ret[i] = vars[i].get(GRB_DoubleAttr_X);
		}
		return ret;
	}
	catch(GRBException e){
		std::cout << "Error Code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
}

std::vector<double> game_map::solve_game_ambusher(){
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		GRBVar *vars = new GRBVar[grid.size()];
		for (int i = 0; i < grid.size(); i++) {
			std::string name = "p_" + itos(i);
			vars[i] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "q" + itos(i));
		}
		GRBLinExpr obj_function = 0;
		for (int i = 0; i < grid.size(); i++) {
			GRBLinExpr intermediate = 0;
			for (int j = 0; j < edges.size(); j++) {
				intermediate += connectivity_matrix[i * edges.size() + j] * solution[j]*vars[i];
			}
			obj_function += intermediate;
		}
		model.setObjective(obj_function, GRB_MAXIMIZE);

		GRBLinExpr measure_const = 0;
		for (int i = 0; i < grid.size(); i++) {
				measure_const += vars[i];
		}
		model.addConstr(measure_const == 1.0, "m");



		model.optimize();

		std::vector<double> ret(grid.size());
		for (int i = 0; i < grid.size(); i++) {
			ret[i] = vars[i].get(GRB_DoubleAttr_X);
		}
		return ret;
	}
	catch(GRBException e){
		std::cout << "Error Code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
}

std::vector<int> game_map::solve_game_hider() {
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		GRBVar *vars = new GRBVar[grid.size()];
		for (int i = 0; i < grid.size(); i++) {
			std::string name = "p_" + itos(i);
			vars[i] = model.addVar(0.0, 1.0, 0.0, GRB_INTEGER, "q" + itos(i));
		}


		GRBQuadExpr obj_function = 0;
		for (int i = 0; i < grid.size(); i++) {
			for (int j = 0; j < grid.size(); j++) {
				obj_function += distance_matrix[i * grid.size() + j] * vars[j] * vars[i];
			}
		}
		model.setObjective(obj_function, GRB_MAXIMIZE);


		GRBLinExpr number_targets_const = 0;
		for (int i = 0; i < grid.size(); i++) {
				 number_targets_const += vars[i];
		}
		model.addConstr(number_targets_const == 4.0, "m");



		model.optimize();

		std::vector<int> ret(grid.size());
		for (int i = 0; i < grid.size(); i++) {
			ret[i] = vars[i].get(GRB_DoubleAttr_X);
		}
		return ret;
	}
	catch(GRBException e){
		std::cout << "Error Code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
}

std::vector<double> game_map::solve_game_searcher() {
	return std::vector<double>();
}

double game_map::game_outcome(){
	std::vector<double> intermediate_vec(grid.size());
	for(int i = 0; i < grid.size(); i++){
		intermediate_vec[i] = 0;
		for(int j = 0; j < edges.size(); j++){
			intermediate_vec[i] += D_matrix[i*edges.size() + j] *solution[j];
		}
	}
	double ret = 0;
	for(int i = 0; i < grid.size(); i++){
		ret += intermediate_vec[i]*ambush[i];
	}
	return ret;
}

std::vector<std::vector<int> > game_map::generate_adjacency_powers() {
	std::vector<std::vector<int>> ret(grid.size());
	ret[0] = adjacency_matrix;
	for(int z = 1; z < grid.size(); z++) {
		std::vector<int> next_power(grid.size()*grid.size(), 0);
		for(int i = 0; i < grid.size(); i++) {
			for(int j = 0; j < grid.size(); j++) {
				for(int k = 0; k < grid.size(); k++) {
					next_power[i*grid.size() + j] += ret[0][i*grid.size() + k]*ret[z-1][k*grid.size() + j];
				}
			}
		}
		ret[z] = next_power;
	}
	return ret;

}

int game_map::distance_between_vertices(int i, int j) {
	for(int z = 0; z < grid.size(); z++){
		if(adjacency_matrix_powers[z][i*grid.size() + j] >= 1){
			return z + 1;
		}
	}
}

int * game_map::generate_distance_matrix() {
	int * ret = (int *) malloc(sizeof(int)*grid.size()*grid.size());
	for(int i = 0; i < grid.size(); i++) {
		for(int j = 0; j < grid.size(); j++) {
			ret[i*grid.size() + j] = distance_between_vertices(i,j);
		}
	}
	return ret;
}