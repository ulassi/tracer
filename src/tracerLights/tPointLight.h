#pragma once
#include "tlightsource.h"
#include <vec3.h>
#include "typedefs.h"
#include "tColor.h"
#include "tMath.h"

class tPointLight :
	public tLightSource
{
public:
	vec3		position;
	tColor		color;
	double		falloffDistance;

	tPointLight(void) :
		color(tColor(1)), 
		position(vec3(0,1,0)),
		falloffDistance(LARGE_VAL)
		{
		lType = POINT_LIGHT;
		noSamples = 1;
	};
	tPointLight(const tPointLight &o) { *this = o; }
    
	tPointLight & operator= (const tPointLight &right) {
		this->position = right.position;
		this->color = right.color;
		this->noSamples = right.noSamples;
		return *this;
	};
	tPointLight * clone() const {
		return new tPointLight(*this);
	};

	virtual tColor	intensity(const vec3 &worldPoint) {
		double d = (worldPoint - position).lengthSquared();
		double one_over_nsamples = 1/double(this->noSamples);
		//linear falloff
		return tColor(1 - min(1,fdiv(d,falloffDistance*falloffDistance)))*this->color*one_over_nsamples;
	};

	
	virtual void sampleLight(vec3 &lightSamplePoint_worldcoords, tColor &colorAtPoint) {
		lightSamplePoint_worldcoords = position;
		double one_over_nsamples = 1/double(this->noSamples);
		colorAtPoint = color*one_over_nsamples;
	};

	~tPointLight(void){};
};
