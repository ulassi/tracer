#pragma once
#include "tshaderoperator.h"
#include <algorithm>
/*
	The shader operator +, for any plugs

	Add a plug / shader by calling connect with a 
	custom string (must be unique for the shader node).
*/


class tShaderOpAdd :
	public tShaderOperator
{
public:
	tShaderOpAdd(void){
	};
	tShaderOpAdd(const tShaderOpAdd &s) {
		*this = s;
	};
	tShaderOpAdd & operator= (const tShaderOpAdd &o) {
		this->inputPlugs = o.inputPlugs;
		this->plugNames = o.plugNames;
		return *this;
	};
	virtual ~tShaderOpAdd(void) {};
	tShaderOpAdd * clone() const {
		return new tShaderOpAdd(*this);
	};
	
	virtual tColor evaluate(const tShaderData &data) {
		tColor sum(0);
		std::vector<std::string>::iterator strIt;
		strIt = plugNames.begin();
		while (strIt != plugNames.end()) {
			sum += inputPlugs.evaluatePlug((*strIt), data);
			strIt++;
		}
		return sum;
	};

	virtual tColor transmit(const tShaderData &data) {
		tColor sum(0);
		std::vector<std::string>::iterator strIt;
		strIt = plugNames.begin();
		while (strIt != plugNames.end()) {
			sum += inputPlugs.transmitPlug((*strIt), data);
			strIt++;
		}
		return sum;	
	};
};
