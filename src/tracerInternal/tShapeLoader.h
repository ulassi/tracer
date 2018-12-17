#pragma once
#include <LWOReader.h>
#include "tPolyShape.h"
/*
	tShapeLoader class
		Wrapper for different loaders.
		Takes a filenmae and a tPolyShape and loads file into shape.

		Supported formats:
			LWO2		
*/
class tShapeLoader
{
public:
	tShapeLoader(void);
	~tShapeLoader(void);

	int load(std::string file, tPolyShape &shape);
private:
	LWOReader readerLWO;

	int makeShapeFromLWO(LWOobj &obj, tPolyShape &shape);
};
