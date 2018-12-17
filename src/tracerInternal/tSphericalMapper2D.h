#pragma once
#include "stdafx.h"
#include "tuvmapper2d.h"

/*
	Class tSphericalMapper2D
		Maps <theta,phi> -> <s,t> 2D tex coords
		Maps <x,y,z>	-> <theta,phi> -> <s,t> 2D tex coords 
*/
class tSphericalMapper2D :
	public tUVmapper2D
{
public:
	tSphericalMapper2D(void);
	tSphericalMapper2D(const tSphericalMapper2D &m);
	tSphericalMapper2D & operator= (const tSphericalMapper2D &m);
	virtual ~tSphericalMapper2D(void);

	virtual tSphericalMapper2D * clone() const;

	virtual double2 getMappingCoords(const std::vector<double> &input) const;
};
