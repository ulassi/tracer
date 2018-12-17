#include ".\ttracer.h"
#include "typedefs.h"
#include "tShaderData.h"
#include "tIntersection.h"
#include "tScene.h"
#include <iostream>

tTracer::tTracer(void) : currentScene(0), mBackgroundCol(tColor(0.2)), mRenMode(0), mMaxDepth(4)
{
}

tTracer::~tTracer(void)
{
}
tTracer & tTracer::operator =(const tTracer &right) {
	this->currentScene = right.currentScene;
	this->gRaysShot = right.gRaysShot;
	this->mBackgroundCol = right.mBackgroundCol;
	this->mMaxDepth = right.mMaxDepth;
	this->mRenMode = right.mRenMode;
	return *this;
}
void tTracer::setRenderMode(int mode) {
	mRenMode = mode;
}
void tTracer::setMaxRecursiveDepth(unsigned int depth) {
	mMaxDepth = depth;
}
int tTracer::render(tFrame &rTarget, tScene &rScene, unsigned int cameraIndex, unsigned int samplesPerPixel){
	int status = 1;
	gLog << " Rendering frame " << std::endl;
	
    //render to rTarget
	currentScene = &rScene;
	if (rScene.getCameras().size() == 0) {
		tCamera camDefault;
		camDefault.setImageWidth(rTarget.image.getWidth());
		camDefault.setImageHeight(rTarget.image.getHeight());
        rScene.addCamera(&camDefault);
		cameraIndex = 0;
	}
	
	/*
		For all pixels, P, in image
	*/
	unsigned int NUM_SAMPLES = (samplesPerPixel > 0) ? samplesPerPixel:1;
	rScene.getCameras().at(cameraIndex)->setSampleGridSize((unsigned int) sqrt((double)NUM_SAMPLES));
	rScene.getCameras().at(cameraIndex)->setImageWidth(rTarget.image.getWidth());
	rScene.getCameras().at(cameraIndex)->setImageHeight(rTarget.image.getHeight());
	double one_over_num_samples = 1.0/(double)NUM_SAMPLES;
	
	//Do a progress bar in output window
	float noOfSteps = 60;
	float stepSize = rTarget.image.getWidth()/static_cast<float>(noOfSteps);
	float step = 0.0f;
	std::cout << "|";
	for(int c=0;c<noOfSteps;c++)
		std::cout << " ";
	std::cout << "|\n|>";

	for (unsigned int x=0; x<rTarget.image.getWidth(); x++) {
		for (unsigned int y=0; y<rTarget.image.getHeight(); y++) {
			for (unsigned int s=0; s<NUM_SAMPLES; s++) {
			tColor C;
			
			/*	generate a ray R into scene, and find color for it */
			C = RayTrace(rScene.getCameras().at(cameraIndex)->getRay(x,y),0);

			/* store pixel color */
			rTarget.image.setPixel(x,y, 
				rTarget.image.samplePixel(x,y) + C*one_over_num_samples);
			}
		}
		step += 1.0f;
		if(step > stepSize)
		{
			std::cout << "\b=>";
			step -= stepSize;
		}
	}
	std::cout << "|" << std::endl;

	gLog << " Done rendering : ";
	if (status) 
		gLog << "Success." << std::endl;
	else
		gLog << ERRSTR << 
		"Failed, code " << status << std::endl;
	return status;
}

tColor	tTracer::RayTrace(const tRay &ray, unsigned int depth) const {
	tColor C;
    /*
	Pseudocode from: http://fuzzyphoton.tripod.com/rtalgo.htm
	Procedure RayTrace(ray R, integer Depth) returns colour
	Set the numerical variable Dis to a maximum value
	Set the object pointer Obj to null
	*/

	/*
	For each object in the scene
	*/
	if (currentScene != 0) {
		//tTracer::gRaysShot += 1.0;
		std::vector<tShape*>::iterator objIt = currentScene->getShapes().begin();
		double distToObj = LARGE_VAL;
		tShape *obj = 0;
		tIntersection currentIntersection;
		if (depth >= mMaxDepth)
			return mBackgroundCol;
		
		while (objIt != currentScene->getShapes().end()) {
			/*
			Calculate the distance (from the starting point of R) of the nearest
			intersection of R with the object in the forward direction
			*/
			tIntersection hitInfo;
			double dist = (*objIt)->intersection(ray,hitInfo);
			
			
			//If this distance is less than Dis
			if ( (dist > SMALL_VAL) && dist < distToObj) {
				//Update Dis to this distance
				//Set Obj to point to this object
				distToObj = dist;
				currentIntersection = hitInfo;
				obj = (*objIt);
				if (depth > 0)
					bool bajs = true;
			}
			objIt++;
		}
		
		//If Obj is not null
		if (obj != 0) {
			vec3 E = ray.dir*-1;
			E.normalize();
			//Set the position variable Pt to the nearest intersection point of R and Obj
			//Set the total colour C to black
			C = 0.0;
			
			//For each light source in the scene
			std::vector<tLightSource*>::iterator lightIt = currentScene->getLights().begin();
			
			while (lightIt != currentScene->getLights().end()) {
				/*
					For each object in the scene
				*/
				vec3 L;
				tColor lightCol, shadowColor, totalShadowAmount(0);

				unsigned int N = (*lightIt)->getNumberOfSamples();

				//Sample lightsource
				for (unsigned int lsamp=0; lsamp < N; lsamp++) {
					shadowColor = tColor(1);		
					(*lightIt)->sampleLight(L, lightCol);		//light in worldspace
					L = L - currentIntersection.P;
					double distanceToLight = L.length();
					L.normalize();	//find normalized light direction
					
					if ( (mRenMode & TRACER_SHADOWS) > 0) {
						/*
						Check for shadows
						*/
						objIt = currentScene->getShapes().begin();
						while (objIt != currentScene->getShapes().end()) {
							
							if ((*objIt) != obj) {
																
								//Create a Ray from pos to lightsource, start with pointlights
								tIntersection shadowHit;
								tRay shadowRay(currentIntersection.P, L);
							
								double t = (*objIt)->intersection(shadowRay,shadowHit);					
								/*
								If this object blocks the light coming from the light source to Pt
								*/
								
								if (t > SMALL_VAL && t<= distanceToLight) {
									/*
									Attenuate the intensity of the received light by the transmittivity
										of the object
									*/
									tShaderData data(L, lightCol, E, *this, depth, shadowHit);
									//returns ZERO for opaque objects, ONE for total transparency
									shadowColor = (*objIt)->getShader()->transmit(data);
								}
							}
							objIt++;
						}//Next object
					}
					tColor colAtP(0);
					/*
					Calculate the perceived colour of Obj at Pt due to this light source
						using the value of the attenuated light intensity
					*/
					if (obj->getShader() != NULL) {
						tShaderData data(L, lightCol*shadowColor,E, *this, depth, currentIntersection);
						colAtP = obj->getShader()->evaluate(data);
					}
					else {
						gLog << "tTracer::RayTrace error : no shader in object." << std::endl;
					}
					/*
					Add this colour contribution to C
					*/
					C += colAtP;//*(tColor(1) - transp);
				}
				lightIt++;
			}//Next light source

		}
		else {
			// Set C to background color
			C = mBackgroundCol;
		}
		
	}
	else
		gLog << " ERROR : tTracer::RayTrace() : no scene defined. " << std::endl;
	
	return C;
}
//Reflects this vector in a plane with normal n
void tTracer::reflect(const vec3 &N, const vec3 &I, vec3 &reflection) const {
	//reflection = I - I*(2.0*(N , N));//R = I - 2 (N NT) I
	// ------REFLECTION----------------------- 
    // New vector R = reflection of I about N.
	// new HYPERFAST reflection
    double two_ndoti = 2 * (N,I); // 4 muls, 2 adds
    reflection.set(two_ndoti * N[0] - I[0],                        // 3 muls, 3 adds
		two_ndoti * N[1] - I[1],                        // --total-------
		two_ndoti * N[2] - I[2]);                        // 7 muls, 5 adds    
}
//Refracts I in surface with normal N and index of refraction ior (normally >=1.0)
void tTracer::refract(const vec3 &N, const vec3 &I, vec3 &refraction, double ior) const {
	// b = ior, -/+ means use minus when ndoti>=0, and use plus otherwise
	//REFRACTION
	//T = (ior * (N.I) -/+ sqrt(1 - ior^2*(1-(N.I)^2))*N - ior*I
	//((ni / nr) (N.L) - sqrt(1 - (ni / nr)2 (N.L)2) N - (ni / nr) L
	double ndoti = (N,I);
	double intoSQRT = 1 - ior*ior*(1-ndoti*ndoti);
	int mul = 1;
	if (ndoti >= 0)
		mul*=-1;
	if (intoSQRT >= 0) {
		refraction = (ior*ndoti + mul* sqrt(intoSQRT))*N - ior*I;
	}
	else {
		//Total inner reflection
		refraction = 2*ndoti*N - I;
	}
}

