#pragma once
#include <vector>
#include <tLightSource.h>
#include <tColor.h>
#include <typedefs.h>
#include <time.h>

/*
	This is a rectangular Area light.

	Sampled just like a point-light, except the point is
	jittered on the area of the light.
*/
class tAreaLight :
	public tLightSource
{
public:
	tColor lightCol;

	tAreaLight(void) : 
		center(vec3(0,0,-5)), 
		tangentX(vec3(1,0,0)), 
		tangentY(vec3(0,0,1)), 
		xSize(2), 
		ySize(2), 
		lightCol(tColor(1)) {this->lType = AREALIGHT;	};

	tAreaLight(const tAreaLight &l) { *this = l; };
	virtual ~tAreaLight(void) {};
	tAreaLight & operator= (const tAreaLight &l) {
		center = l.center;
		tangentX = l.tangentX;
		tangentY = l.tangentY;
		xSize = l.xSize;
		ySize = l.ySize;
		samples = l.samples;
		noSamples = l.noSamples;
		lightCol = l.lightCol;
		return *this;
	};
	tAreaLight *clone() const {
		return new tAreaLight(*this);
	};
	/*
		Returns a attenuated color at a point in world space
	*/
	virtual tColor	intensity(const vec3 &worldPoint) {
		return tColor(1/double(noSamples));	
	};
	/*
		Return world space position to a sample point for light
	*/
	virtual void sampleLight(vec3 &lightSamplePoint_worldcoords, tColor &colorAtPoint) {
		
		if (sampled >= noSamples -1) 
			sampled = 0;
		//x,y in range [0...1]
		double2 coord;
		unsigned int samplesPerSide = (unsigned int) sqrt((double)noSamples);
		double one_over_nsamples = 1.0/double(samplesPerSide);
		unsigned int y_offset = sampled/samplesPerSide, x_offset = sampled%samplesPerSide; 
		double x = (double(x_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
		double y = (double(y_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
		coord.x = -this->xSize/2.0 + x*this->xSize;
		coord.y = -this->ySize/2.0 + y*this->ySize;
		sampled++;
		lightSamplePoint_worldcoords = center + tangentX*coord.x + tangentY*coord.y;
		colorAtPoint = this->lightCol*one_over_nsamples*one_over_nsamples;
	};
	void setNumberOfSamples(unsigned int N) {
		noSamples = N;
	};
	/*
		Defines the plane on which it lies, and size of the area light
	*/
	void setSpanAndPlane(const vec3 &_tangentX, const vec3 &_tangentY, const vec3 &_center,const double &_xSize, const double &_ySize) {
        xSize = _xSize;
		ySize = _ySize;
        center = _center;
		tangentX = _tangentX;
		tangentY = _tangentY;
	}
private:
    vec3 center;				//centerpoint
	vec3 tangentX, tangentY;	//span of plane
	double xSize, ySize;
	unsigned int sampled;
	std::vector<double2> samples;
};
