#pragma once
#include <vector>
/*
	Abstract 2D uvmapper class
*/
class tUVmapper2D
{
public:
	//data members
	double2 wrapAmount;	//number of times wrapped on U,V -> (x,y)
	double2 offset;		//offset amount U,V -> (x,y)
    
	tUVmapper2D(void) { wrapAmount.x = wrapAmount.y = 1; offset.x = offset.y = 0;};
	virtual ~tUVmapper2D(void) {};
    virtual tUVmapper2D * clone() const = 0;

	//TODO: verify this interface
	virtual double2 getMappingCoords(const std::vector<double> &input) const = 0;
};
