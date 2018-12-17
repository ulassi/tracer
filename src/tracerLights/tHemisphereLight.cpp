#include "./themispherelight.h"
#include <stdlib.h>
#include <typedefs.h>
tHemisphereLight::tHemisphereLight(void) : sampled(0), constantColor(1) {
	this->setNumberOfSamples(500);
}
tHemisphereLight::tHemisphereLight(const tHemisphereLight &l) {
	*this = l;
}
tHemisphereLight::~tHemisphereLight(void){}
tHemisphereLight * tHemisphereLight::clone() const {
	return new tHemisphereLight(*this);	
}
tHemisphereLight & tHemisphereLight::operator= (const tHemisphereLight &l) {
	//this->samples = l.samples;
	this->noSamples = l.noSamples;
	this->sampled = l.sampled;
	this->constantColor = l.constantColor;
	return *this;
}
/*
	Returns a attenuated color at a point in world space
*/
tColor	tHemisphereLight::intensity(const vec3 &worldPoint) {
	return tColor(1);
}
/*
	Return world space position to a sample point for light
*/
void tHemisphereLight::sampleLight(vec3 &lightSamplePoint_worldcoords, tColor &colorAtPoint) {
	if (sampled >= noSamples-1) 
		sampled = 0;
	//x,y in range [0...1]
	unsigned int samplesPerSide = (unsigned int) sqrt((double)noSamples);
	double one_over_nsamples = 1.0/double(samplesPerSide);
	unsigned int y_offset = sampled/samplesPerSide, x_offset = sampled%samplesPerSide; 
	double x = (double(x_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
	double y = (double(y_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
	double phi = 2.0f * acos(sqrt(1.0f - y));
	double theta = PI*2.0f * x;
	lightSamplePoint_worldcoords.set(sin(phi) * cos(theta),
		sin(phi) * sin(theta),
		cos(phi));
	lightSamplePoint_worldcoords *= LARGE_VAL;
	colorAtPoint = constantColor*one_over_nsamples*one_over_nsamples;
	sampled++;
}

void tHemisphereLight::setNumberOfSamples(unsigned int N) {
	unsigned int sqrtN = (unsigned int) sqrt(double(N));
	noSamples = sqrtN*sqrtN;
}