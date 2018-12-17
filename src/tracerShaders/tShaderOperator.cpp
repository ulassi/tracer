#include ".\tshaderoperator.h"

tShaderOperator::tShaderOperator(void)
{
}

tShaderOperator::~tShaderOperator(void)
{
}
tShaderOperator::tShaderOperator(const tShaderOperator &c) {
	*this = c;
}

tShaderOperator & tShaderOperator::operator= (const tShaderOperator &right) {
	this->inputPlugs = right.inputPlugs;
	this->plugNames = right.plugNames;
	return *this;
}
