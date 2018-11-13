//
// Created by jbm on 11/9/18.
//

#include "point.h"

point::point(double x_, double y_) : x(x_), y(y_) {}

//grid starts at 0
point::point(point && rhs) noexcept : x(rhs.x), y(rhs.y)  {
	rhs.x = -1;
	rhs.y = -1;
}

point::point(const point & rhs) : x(rhs.x), y(rhs.y) {}

point& point::operator=(const point & rhs) {
	this->x = rhs.x;
	this->y = rhs.y;
	return *this;
}

point& point::operator=(point && rhs) noexcept{
	this->x = rhs.x;
	this->y = rhs.y;
	rhs.x = 0;
	rhs.y = 0;
	return *this;
}

point::point(std::initializer_list<double> list) {
	auto list_it = list.begin();
	this->x = *list_it;
	this->y = *++list_it;
}

bool point::operator==(const point & rhs) {
	if(this->x == rhs.x && this->y == rhs.y)
		return true;
	else
		return false;
}

double point::operator^(const point & rhs) {
	return sqrt((this->x - rhs.x)*(this->x - rhs.x) + (this->y - rhs.y)*(this->y - rhs.y));
}