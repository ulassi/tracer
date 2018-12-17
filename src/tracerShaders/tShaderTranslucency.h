#pragma once
#include "tshader.h"


class tShaderTranslucency :
	public tShader
{
public:
	//PLUG LIST
	const static std::string plugTransparencyColor;
	const static std::string plugTransparency;
	const static std::string plugIor;

	tShaderTranslucency(void) {
		inputPlugs.addPlug(plugTransparencyColor);
		inputPlugs.addPlug(plugTransparency);
		inputPlugs.addPlug(plugIor);
	};
	tShaderTranslucency(const tShaderTranslucency &s) { *this = s;};
	tShaderTranslucency & operator= (const tShaderTranslucency &s) {
		this->inputPlugs = s.inputPlugs;
		noSamples = s.noSamples;
		return *this;
	};
	virtual ~tShaderTranslucency(void) {};
	tShaderTranslucency * clone() const { return new tShaderTranslucency(*this); };
	virtual tColor evaluate(const tShaderData &data) 
	{
		vec3 R;
		data.renObj.refract(data.intersection.N, 
			data.viewVec,
			R,
			inputPlugs.evaluatePlug(plugIor, data));
						
		//jitter R
		//convert R to theta phi:
		double phi0 = acos(R[2]), phi;
		double theta0 = atan(fdiv(R[1], R[0])), theta;

		//jitter theta, phi
		double deltaHalf = PI/double(noSamples);
		//if (sampled >= noSamples -1) 
		//	sampled = 0;
		tColor result(0);
		unsigned int samplesPerSide = (unsigned int) sqrt((double)noSamples);
		double one_over_nsamples = 1.0/double(samplesPerSide);

		for (unsigned int s = 0; s< samplesPerSide*samplesPerSide; s++) {
			unsigned int y_offset = s/samplesPerSide, x_offset = s%samplesPerSide; 
			double x = (double(x_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			double y = (double(y_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			phi = phi0 + -deltaHalf + 2*y*deltaHalf;
			theta = theta0 + -deltaHalf + 2*x*deltaHalf;
			vec3 sampleDir(-sin(phi) * cos(theta),
				-sin(phi) * sin(theta),
				cos(phi));
			tRay newRay(data.intersection.P, sampleDir);
			double EdotR = EdotR = max(0,(sampleDir, data.viewVec*-1));
			if (EdotR > 1)
				bool bajs = 0;
			result += EdotR*data.renObj.RayTrace(newRay, data.depth+1);
		}
		result *= one_over_nsamples;
		//double EdotR = max(0,(viewDirection, R));
		return inputPlugs.evaluatePlug(plugTransparencyColor, data)*
			inputPlugs.evaluatePlug(plugTransparency, data)*
			result;// pow(EdotR,(double)n) ;
	};

	virtual tColor transmit(const tShaderData &data) {
		return inputPlugs.transmitPlug(plugTransparencyColor, data)*
			inputPlugs.transmitPlug(plugTransparency, data);
	};
	/*
		Sets number of samples to : sqrt(N)*sqrt(N)
		TODO: replace this with a sampler2D
	*/
	void setNumberOfSamples(unsigned int N) {
		noSamples = N;
	};
private:
	unsigned int noSamples;
};
const std::string tShaderTranslucency::plugTransparencyColor = std::string("transpCol");
const std::string tShaderTranslucency::plugTransparency = std::string("transp");
const std::string tShaderTranslucency::plugIor = std::string("ior");
