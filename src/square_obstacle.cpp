//
// Created by jbm on 11/9/18.
//


#include "square_obstacle.h"

square_obstacle::square_obstacle(point tl, point tr, point bl, point br) : top_left(tl),
																			top_right(tr),
																			bottom_left(bl),
																			bottom_right(br) {
	this->width = tl.x - tr.x;
	this->height = tl.y - bl.y;
}


square_obstacle::square_obstacle(point tl, double w_, double h_) : top_left(tl),
																	width(w_),
																	height(h_) {

	this->top_right = point(tl.x + w_, tl.y);
	this->bottom_right = point(tl.x + w_, tl.y - h_);
	this->bottom_left = point(tl.x, tl.y - h_);
}

square_obstacle::square_obstacle(const square_obstacle & rhs) : top_left(rhs.top_left),
																top_right(rhs.top_right),
																bottom_left(rhs.bottom_left),
																bottom_right(bottom_right),
																width(rhs.width),
																height(rhs.height) {}
square_obstacle::square_obstacle(square_obstacle && rhs) noexcept {
	this->top_left = std::move(rhs.top_left);
	this->top_right = std::move(rhs.top_right);
	this->bottom_left = std::move(rhs.bottom_left);
	this->bottom_right = std::move(rhs.bottom_right);
	rhs.width = -1;
	rhs.height = -1;
}

square_obstacle& square_obstacle::operator=(const square_obstacle & rhs) {
	this->top_left = rhs.top_left;
	this->top_right = rhs.top_right;
	this->bottom_left = rhs.bottom_left;
	this->bottom_right = rhs.bottom_right;
	this->width = rhs.width;
	this->height = rhs.height;

	return *this;
}

square_obstacle& square_obstacle::operator=(square_obstacle && rhs) noexcept{
	this->top_left = std::move(rhs.top_left);
	this->top_right = std::move(rhs.top_right);
	this->bottom_left = std::move(rhs.bottom_left);
	this->bottom_right = std::move(rhs.bottom_right);
	this->width = rhs.width;
	this->height = rhs.height;

	rhs.width = -1;
	rhs.height = -1;

	return *this;
}

square_obstacle::square_obstacle(std::initializer_list<point> list) {
	auto list_it = list.begin();
	this->top_left = *list_it;
	this->top_right = *++list_it;
	this->bottom_left = *++list_it;
	this->bottom_right = *++list_it;
}