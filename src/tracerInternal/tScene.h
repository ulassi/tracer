#pragma once
#include <vector>
#include "tShape.h"
#include "tLightSource.h"
#include "tCamera.h"
/*
	A object that holds all the scene data, and
	acts as an query interface to the scene data.
*/
class tScene
{
public:
	tScene(void);
	~tScene(void);
	/*
		Need to handle pointer because of polymorphism!
	*/
	void addShape(tShape *shapePtr);
	void addCamera(tCamera *camPtr);
	void addLight(tLightSource *light);

	//TEMP lazy methods
	std::vector<tShape*> & getShapes();
	std::vector<tCamera*> & getCameras();
	std::vector<tLightSource*> & getLights();
private:
	std::vector<tShape*>		shapes;
	std::vector<tLightSource*>	lights;
	std::vector<tCamera*>		cameras;
};
