#include ".\tshaderopmult.h"

tShaderOpMult::tShaderOpMult(void) {

}
tShaderOpMult::tShaderOpMult(const tShaderOpMult &c) {
	*this = c;
}
tShaderOpMult::~tShaderOpMult(void) {
}

tShaderOpMult & tShaderOpMult::operator=(const tShaderOpMult &right) {
	this->plugNames = right.plugNames;
	this->inputPlugs = right.inputPlugs;
    return *this;
}
tShaderOpMult *tShaderOpMult::clone() const {
	return new tShaderOpMult(*this);
}

tColor tShaderOpMult::evaluate(const tShaderData &data) {
	tColor result(1);
	std::vector<std::string>::iterator strIt;
	strIt = plugNames.begin();
	while (strIt != plugNames.end()) {
		result *= inputPlugs.evaluatePlug((*strIt), data);
		strIt++;
	}
	return result;
}
tColor tShaderOpMult::transmit(const tShaderData &data){
	tColor result(1);
	std::vector<std::string>::iterator strIt;
	strIt = plugNames.begin();
	while (strIt != plugNames.end()) {
		result *= inputPlugs.evaluatePlug((*strIt), data);
		strIt++;
	}
	return result;
}
