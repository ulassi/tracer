#include ".\tscene.h"

tScene::tScene(void)
{

}

tScene::~tScene(void)
{
	while (shapes.size() > 0) {
		tShape *s = shapes.back();
		shapes.pop_back();
		delete s;
	}
	while (lights.size() > 0) {
		tLightSource *s = lights.back();
		lights.pop_back();
		delete s;
	}
	while (cameras.size() > 0) {
		tCamera *s = cameras.back();
		cameras.pop_back();
		delete s;
	}
		
}
void tScene::addShape(tShape *shapePtr) {
	tShape *t = shapePtr->clone();		//need to use clone since tShape is abstract
	t->init();
	shapes.push_back(t);
}
void tScene::addCamera(tCamera *camPtr) {
	tCamera *c = new tCamera(*camPtr);
	cameras.push_back(c);
}
void tScene::addLight(tLightSource *light) {
	tLightSource *l = light->clone();	//need to use clone since tLightSource is abstract
	lights.push_back(l);
}
std::vector<tShape*> & tScene::getShapes() {
	return shapes;
}
std::vector<tCamera*> & tScene::getCameras() {
	return cameras;
}
std::vector<tLightSource*> & tScene::getLights() {
	return lights;
}