#include ".\tshadertextureimage2d.h"
#include "tIntersection.h"

tShaderTextureImage2D::tShaderTextureImage2D(void)
{
}

tShaderTextureImage2D::~tShaderTextureImage2D(void)
{
}
tShaderTextureImage2D::tShaderTextureImage2D(const tImage &img) {
	this->setImage(img);
}
tShaderTextureImage2D::tShaderTextureImage2D(const tShaderTextureImage2D &s) {
	*this = s;
}
tShaderTextureImage2D & tShaderTextureImage2D::operator= (const tShaderTextureImage2D &right) {
	this->inputPlugs = right.inputPlugs;
	this->texture = right.texture;
	return *this;
}
tShaderTextureImage2D * tShaderTextureImage2D::clone() const {
	return new tShaderTextureImage2D(*this);
};

/*
	Returns a color for a point.
*/
tColor tShaderTextureImage2D::evaluate(const tShaderData &data) 
{
	if (data.intersection.uvs.size() >= 1) {
		return texture.sample(data.intersection.uvs.at(0).x,data.intersection.uvs.at(0).y);
	}
	else 
		return tColor(0);
}
/*
	Returns a color value used for shadows.
*/
tColor tShaderTextureImage2D::transmit(const tShaderData &data) 
{
	return tColor(0);
}

void tShaderTextureImage2D::setImage(const tImage &img) {
	this->texture.setImage(img);
}