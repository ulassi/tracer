#include "tshaderdiffuse.h"

const std::string tShaderDiffuse::plugDiffuseColor = std::string("diffusecolor");
//implemenatation of methods
tShaderDiffuse::tShaderDiffuse(void){
	this->inputPlugs.connect(plugDiffuseColor, &tShaderConstant(tColor(0.7)));
}
tShaderDiffuse::tShaderDiffuse(const tColor &col) {
	this->inputPlugs.connect(plugDiffuseColor, &tShaderConstant(col));
}
tShaderDiffuse::tShaderDiffuse(const tShaderDiffuse &s) { *this = s;}

tShaderDiffuse & tShaderDiffuse::operator= (const tShaderDiffuse &s) {
	this->inputPlugs = s.inputPlugs;
	return *this;
}
tShaderDiffuse::~tShaderDiffuse(void) {

}
tShaderDiffuse * tShaderDiffuse::clone() const { 
	return new tShaderDiffuse(*this); 
}