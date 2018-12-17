#pragma once
#include "tshader.h"
#include <vector>
/*
	Class tMultiSurfaceShader:

	Purpose of this is to be used with a a tPolyShape object with multiple
	surfaces. If a hit is scored a surfaceIndex will be stored in the
	tIntersection class, this can be used with this shader to find
	the appropriate shader for that surface.
*/
class tMultiSurfaceShader :
	public tShader
{
public:
	tMultiSurfaceShader(void){};
	tMultiSurfaceShader(const tMultiSurfaceShader &s) {
		*this = s;
	};
	tMultiSurfaceShader & operator= (const tMultiSurfaceShader &o) {
		size_t i=0;
		while (i<o.mVecShaders.size()) {
			mVecShaders.push_back(o.mVecShaders[i]->clone());			
			i++;
		}
		return *this;
	};
	virtual ~tMultiSurfaceShader(void) {
		while (mVecShaders.size() > 0) {
			tShader * sh = mVecShaders.back();
			mVecShaders.pop_back();
            delete sh;
		}
	};
	tMultiSurfaceShader * clone() const {
		return new tMultiSurfaceShader(*this);
	};
	/*
		Clones sh and puts it on the back internal surface shader vector
	*/
	void addShader(tShader *sh){
		mVecShaders.push_back(sh->clone());
	};
	virtual tColor evaluate(const tShaderData &data)
	{
		return mVecShaders.at(data.intersection.surfaceIndex)->evaluate(data);	
	};

	virtual tColor transmit(const tShaderData &data) 
	{
		return mVecShaders.at(data.intersection.surfaceIndex)->transmit(data);	
	};
private:
	std::vector<tShader *> mVecShaders;
};
