/*
	raytrace.cpp : Defines the entry point for the application.
		version: 0.06 2004 - xxx
			* fixed *-1 error in tShaderRefraction::evaluate()
			* removed the '-fsaa' switch
		v 0.05
			* added xml scene parsing, does not yet support shaders to ful extent
				parser supports:
					lights: point, area, hemi
					shapes: sphere, plane
					shaders: diffuse, specular (basically phong combination), 
				parser CANNOT handle nested tags of the same name FIX THAT!
		v 0.03
			* redesign of shaders, a whole new interface. Uses connect() and tShaderPlugs to
			connect shaders to each others.
			* moved averaging of lightColor / intensity into lightsource implementation. 
			Used to be in tTracer::RayTrace, bad for control over intensity.
		
*/
//#include <optimize.h>
#include <iostream>
#include <fstream>
#include <string>
#include "tScene.h"
#include "tCamera.h"
#include "tTracer.h"
#include "aWindow.h"
#include "tShapeLoader.h"
#include "tSceneLoader.h"
#include <ImageLib.h>
#include "tSphericalMapper2D.h"
//SHAPES
#include "tPolyShape.h"
#include "tSphere.h"
#include "tInfinitePlane.h"
//LIGHTS
#include "tPointLight.h"
#include "tHemisphereLight.h"
#include "tAreaLight.h"

//SHADERS
#include "tShaderDiffuse.h"
#include "tShaderPhongSpecular.h"
#include "tShaderOpAdd.h"
#include "tShaderOpMult.h"
#include "tShaderHemisphereSampler.h"
#include "tShaderGlossSampler.h"
#include "tShaderTraceRefraction.h"
#include "tShaderTraceReflection.h"
#include "tShaderTextureImage2D.h"
#include "tShaderTranslucency.h"
#include "tShader3DChecker.h"

#include <time.h>
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
//#define USE_WIN_XML
std::ofstream gLog("log.txt", std::ofstream::trunc  | std::ofstream::out);
const std::string HELPSTRING = "Raytrace by Urban Lassi, urban.lassi@spray.se\n 2004\nUsage:\n\traytrace scene.xml [switches]\n\nSwitches:\n-w\twidth of image\n-h\theight of image\n-n\tsamples per pixel (rounded to nearset mutliple of 4)\n/h\tthis help\n";

int main(int argc, const char* argv[])
{
	gLog << std::endl << " trace Started " << std::endl << 
		"------------------------ " << std::endl;
	/*
		Parse input arguments
	*/
	std::string sceneFile;
	int fWidth = 0, fHeight = 0, numSamples = 1;
	for (int i=1; i<argc; i++) {
		std::string argument(argv[i]);
		if (argument.at(0) == '/') {
			if (argument == "/h" || argument == "/H") {
				//Help string
				std::cout << HELPSTRING;
				return 0;
			}
		}
		else if (argument.at(0) != '-') {
			//no switch, assume scene file
			sceneFile = argument;
		}
		else {
			//parse switches
			if (argument == "-w" || argument == "-W") {
				fWidth = atoi(argv[i+1]);
				i++;
			}
			else if (argument == "-h" || argument == "-H") {
				fHeight = atoi(argv[i+1]);
				i++;
			}
			else if (argument == "-n" || argument == "-N") {
				numSamples = atoi(argv[i+1]);
				numSamples *= numSamples;
				i++;
			}	
		}
	}	
	if (fWidth == 0 && fHeight == 0) {
		fWidth = 320;
		fHeight = 240;
	}
	else if (fWidth == 0) {
		fWidth = 4*fHeight/3;
	}
	else if (fHeight == 0) {
		fHeight = 3*fWidth/4;
	}
	/*
		Send scene-file to parser and recieve a scene object back
	*/
	if (sceneFile.empty()) {
		//gLog << __FILE__ << " function: " << __FUNCTION__ << " line: " << __LINE__  <<
		gLog << ERRSTR << 
			"scenefile specified not found" << std::endl;
		return -1;
	}
	
	tScene scene;
	tSceneLoader sceneLoader;
	if (!sceneLoader.load(sceneFile, scene))  {
		gLog << ERRSTR << 
			"failed to load scene " << std::endl;
		return -1;
	}
	/*tSphere sph;
	sph.center.set(-5,1,0);
	sph.r = 1;

	// Karls kristallkula
	tShaderDiffuse *crysDiff = new tShaderDiffuse(tColor(0.1));
	tShaderPhongSpecular *crysSpec = new tShaderPhongSpecular(tColor(0.7), 32);
	tShaderTraceRefraction *crysRefr = new tShaderTraceRefraction(0.9, 1.6);
	tShaderOpAdd *crysReg = new tShaderOpAdd();
	crysReg->connect("Crystal Diffuse", crysDiff);
	crysReg->connect("Crystal Specular", crysSpec);
	tShaderOpAdd *crysShader = new tShaderOpAdd();
	crysShader->connect("Crystal Regular", crysReg);
	crysShader->connect("Crystal refraction", crysRefr);
	sph.setShader(crysShader);
	scene.addShape(&sph);
	delete crysDiff; delete crysSpec; delete crysRefr; delete crysReg;
	delete crysShader;*/


	tTracer renderer;
	renderer.setRenderMode(tTracer::TRACER_SHADOWS);
	renderer.setMaxRecursiveDepth(4);

	//storage for frame image
	tFrame frame;
	frame.image = tImage(fWidth,fHeight, 4);
	
	//time the render
	time_t startTime = time(NULL);
	srand( (unsigned)time( NULL ) );	//init random numbers
	
	//	Render
	renderer.gRaysShot = 0;
	renderer.render(frame, scene,0,numSamples);
    time_t endTime = time(NULL);
	double secs = difftime(endTime, startTime);
	gLog << "Render took: " << secs << "secs." << std::endl;

	//print some stats to screen
	printf(" Rendered frame took: %.0f seconds.\n", secs);
	printf(" Rays shot in total: %.0f .\n", renderer.gRaysShot);

	//show rendered frame
	aWindow win(&frame.image);
	win.createWindow("awindowClass", "Render result", frame.image.getWidth(), frame.image.getHeight(), 32, 0, 0);
	
	while (!win.doQuit()) {
		//TODO: implement faster drawing in window
		win.updateMessages();
	}
	win.killWindow();

	//	Save frame as a .raw file
	FILE *imageFile = fopen("output.raw", "wb");
	if (imageFile) {
		//save
		unsigned char value=0;
		tColor bert;
		for (unsigned int i=0; i<frame.image.getNumberOfPixels(); i++) {
			bert = frame.image.samplePixel(i);
			value = (unsigned char) min(255,(bert.R*255));
			fwrite(&value, 1,1,imageFile);
			value = (unsigned char) min(255,(bert.G*255));
			fwrite(&value, 1,1,imageFile);
			value = (unsigned char) min(255,(bert.B*255));
			fwrite(&value, 1,1,imageFile);
		}
		fclose(imageFile);
	}
	//cleanup
	
	return 0;
}

