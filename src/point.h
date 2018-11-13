//
// Created by jbm on 11/9/18.
//

#ifndef SCAG_POINT_H
#define SCAG_POINT_H


#include <initializer_list>
#include "utils.h"
struct point {
	double x;
	double y;

	point() = default;
	point(double , double);
	point(std::initializer_list<double>);
	point(const point &);
	point(point &&) noexcept;
	point&operator=(const point&);
	point&operator=(point&&) noexcept;
	bool operator==(const point &);
	double operator^(const point &); //norm of distance
};


#endif //SCAG_POINT_H
