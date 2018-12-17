#include ".\stdafx.h"
#include ".\ttexture2d.h"
tTexture2d::tTexture2d(void) {

}
tTexture2d::tTexture2d(const tImage &img) {
	image = img;
}
tTexture2d::tTexture2d(const tTexture2d &tex){
	*this = tex;
}
tTexture2d::~tTexture2d(void) {
	
}
tTexture2d & tTexture2d::operator= (const tTexture2d &right) {
	this->image = right.image;
	return *this;
}

const tColor & tTexture2d::sample(const double &s, const double &t) {
	//convert s and t to x and y, nearest neighbour
	//TODO: sample using bilinear filtering
	double u_clamp, v_clamp;
	int u_int, v_int;
	unsigned int index_x, index_y;
	//clamp to closest u,v integer
	u_int = (int)s;
	v_int = (int)t;
	u_clamp = s - u_int;
	v_clamp = t - v_int;
	if (u_clamp < 0)
		u_clamp += 1.0;
	if (v_clamp < 0)
		v_clamp += 1.0;
	index_x = (unsigned int) (u_clamp*image.getWidth());
	index_y = (unsigned int) (v_clamp*image.getHeight());
	return image.samplePixel(index_x, index_y);	
}
//Data manipulation
void tTexture2d::setImage(const tImage &img){
	image = img;
}
const tImage & tTexture2d::getImage() {
	return image;
}