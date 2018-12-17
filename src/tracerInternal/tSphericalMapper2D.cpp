#include ".\stdafx.h"
#include ".\tsphericalmapper2d.h"
#include <math.h>
#include <assert.h>
tSphericalMapper2D::tSphericalMapper2D(void)
{
}

tSphericalMapper2D::tSphericalMapper2D(const tSphericalMapper2D &m) {
	*this = m;
}
	
tSphericalMapper2D::~tSphericalMapper2D(void)
{
}
tSphericalMapper2D * tSphericalMapper2D::clone() const {
	return new tSphericalMapper2D(*this);
}
/*
	Maps u,v so that (if wrapping is 1,1)
		bottom of image (0,*) is in -y
		top of image (1,*) is in +y
		left and right of image is in +x
		
		
	Input:
		<theta,phi> 
		<x,y,z>
*/
double2 tSphericalMapper2D::getMappingCoords(const std::vector<double> &input) const {
	double2 coords;
	assert(input.size() >= 2 && input.size() <= 3);
	if (input.size() == 3) {
		coords = cart2sph(input.at(0), input.at(1), input.at(2));
	}
	else if (input.size() == 2) {
		coords.x = input.at(0); coords.y = input.at(1);
	}
	/*	
		Normalize coords to 0..1
			Coord.x : theta 0...2Pi, Coords.y : phi 0...Pi
	*/
	coords.x = -fdiv(coords.x, 2.0*PI);
	coords.y = 1 - fdiv(coords.y, PI);

	//add offset and create wrap
	coords.x += this->offset.x;
	coords.y += this->offset.y;
	coords.x *= this->wrapAmount.x;
	coords.y *= this->wrapAmount.y;

	return coords;
}
tSphericalMapper2D & tSphericalMapper2D::operator= (const tSphericalMapper2D &m) {
	this->wrapAmount.x = m.wrapAmount.x;
	this->wrapAmount.y = m.wrapAmount.y;
	this->offset.x = m.offset.x;
	this->offset.y = m.offset.y;

	return *this;
}

