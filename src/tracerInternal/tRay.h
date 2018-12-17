#pragma once
#include <vec3.h>
/*
	A ray class.
		start + t*dir 
*/
class tRay
{
public:
	vec3 start, dir;
	tRay(void){};
	tRay(const tRay &r) { *this = r;};
	tRay(const vec3 &_start, const vec3 &_dir) { start = _start; dir = _dir;};
	~tRay(void){};

	tRay & operator= (const tRay &right) {
		this->start = right.start;
		this->dir = right.dir;
		return *this;
	};
	
	
};
