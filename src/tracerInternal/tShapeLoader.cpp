#include "stdafx.h"
#include ".\tshapeloader.h"
#include "tPolygon.h"
#include "tMultiSurfaceShader.h"
#include "tShaderDiffuse.h"
#include "tShaderPhongSpecular.h"
#include "tShaderOpAdd.h"
#include "tShaderTraceReflection.h"
#include "tShaderTraceRefraction.h"
#include "tShaderConstant.h"
tShapeLoader::tShapeLoader(void)
{
}

tShapeLoader::~tShapeLoader(void)
{
}
int tShapeLoader::load(std::string file, tPolyShape &shape) {
	if (file.substr(file.size()-3, file.size()) == "lwo") {
		//load a LWO
		LWOobj object;
		if (readerLWO.readLWO((char*)file.c_str(), object) && makeShapeFromLWO(object, shape)) {
			//SUCCESS!
			return 1;
		}		
	}
	return 0;
};
//TODO: vertex indicies get screwed up if more than one layer are present
//TODO: implement uv coord loading and texturing for polyshapes
int tShapeLoader::makeShapeFromLWO(LWOobj &obj, tPolyShape &shape) {
	//iterate vertices
	std::vector<float>::iterator vIt = obj.mVertices.begin();
	while (vIt != obj.mVertices.end()) {
		//xyz
		double4 vertex;
		vertex.x = (double)(*vIt++);
		vertex.y = (double)(*vIt++);
		vertex.z = (double)(*vIt++);

		//TODO: optimize sphere placement
		vec3 vert(vertex.x, vertex.y, vertex.z);
		double dist = 
			(shape.boundingSphere.center - vert).lengthSquared() - 
			shape.boundingSphere.r*shape.boundingSphere.r;
		if (dist > 0)
		{	//add dist to radius
			shape.boundingSphere.r = (shape.boundingSphere.center - vert).length()+100*SMALL_VAL;
		}
		vertex.w = 1;//set w=1
		shape.mVertexArray.push_back(vertex);		
	}//done verts

	//iterate polygons
	std::vector<unsigned int>::iterator pIt = obj.mPolygons.begin();
	while (pIt != obj.mPolygons.end()) {
		//first is verts in polygon
		unsigned int numVertsInPoly = (*pIt);
		pIt++;
		tPolygon poly;		
		for (unsigned int i=0; i<numVertsInPoly; i++) {
			poly.vertices.push_back(&shape.mVertexArray.at((*pIt))); //keep pointer to values
			poly.vertexIndicies.push_back((*pIt));					//and keep index
			pIt++;
		}
		pIt++;	//skip the flag

		//define polyNormal and set index in poly
		vec3 firstEdge,lastEdge, N;
		double4 nextLastVert = *poly.vertices.at(numVertsInPoly-2);
		double4 lastVert = *poly.vertices.at(numVertsInPoly-1);
		double4 firstVert = *poly.vertices.at(0);
		double4 secondVert = *poly.vertices.at(1);
		firstEdge.set(secondVert.x - firstVert.x,
			secondVert.y - firstVert.y,
			secondVert.z - firstVert.z);
		lastEdge.set(lastVert.x - nextLastVert.x,
			lastVert.y - nextLastVert.y,
			lastVert.z - nextLastVert.z);
		N = firstEdge * lastEdge;
		N.normalize();
		poly.polyNormal = N;
		shape.mPolygonArray.push_back(poly);
	}//done with polys

	//copy poly to surface indicies
	shape.mPolygonSurfaceIndex.resize(shape.mPolygonArray.size(), 0);
	std::vector<unsigned int>::iterator it = obj.mPolySurface.begin();
	while (it != obj.mPolySurface.end()) {
		unsigned int index = (*it);
		it++;
		shape.mPolygonSurfaceIndex.at(index) = (*it);
		shape.mPolygonArray.at(index).surfaceIndex = (*it);
		unsigned int val = shape.mPolygonSurfaceIndex.at(index);
		it++;
	}
	
	//iterate all surfaces
	std::vector<LWOobj::Surface>::iterator sIt = obj.mSurfaces.begin();
	tMultiSurfaceShader *sh = new tMultiSurfaceShader();
	std::vector<float> edgeAngle;
	while (sIt != obj.mSurfaces.end()) {
		/*
			Mix: 
			if (diffuse > 0) 
				color*diffuse -> tShaderDiffuse
			if (specular > 0)
				specularColorHighLight -> tShaderPhongSpecular
			if (reflect > 0)
				add reflection
			if (transparency > 0)
				add transp

		*/
		LWOobj::Surface &s = (*sIt);
		tShaderDiffuse			*diff=0;
		tShaderPhongSpecular	*spec=0;
		tShaderTraceReflection	*reflectSh=0;
		tShaderTraceRefraction	*refractSh=0;
		tShaderConstant			*luminance = 0;
		if ((*sIt).diffuse > 0) {
			tColor col((*sIt).diffuse*(*sIt).baseColour[0],
				(*sIt).diffuse*(*sIt).baseColour[1],
				(*sIt).diffuse*(*sIt).baseColour[2]);
			diff = new tShaderDiffuse(col);
		}
		if ((*sIt).specular > 0) {
			
			/*
				(*sIt).specularColourHighlight = 0.0 -> white
				(*sIt).specularColourHighlight = 1.0 -> baseColour
			*/
			tColor col;
			unsigned int n;
			col = tColor(
				(*sIt).specular*( (*sIt).specularColourHighlight*(*sIt).baseColour[0] + 
				(1.0 - (*sIt).specularColourHighlight )),
				(*sIt).specular*( (*sIt).specularColourHighlight*(*sIt).baseColour[1] + 
				(1.0 - (*sIt).specularColourHighlight ) ),
                (*sIt).specular*( (*sIt).specularColourHighlight*(*sIt).baseColour[2] + 
				(1.0 - (*sIt).specularColourHighlight ) ));
			n = (unsigned int) ((*sIt).gloss*100);
			spec = new tShaderPhongSpecular(col,n);
		}
		if (s.transparency > 0) {
			refractSh = new tShaderTraceRefraction(s.transparency, s.indexOfRefraction);
		}
		if (s.reflectivity > 0) {
			reflectSh = new tShaderTraceReflection(s.reflectivity);
		}
		if (s.luminance > 0) {
			luminance = new tShaderConstant(tColor(s.baseColour[0], 
				s.baseColour[1], 
				s.baseColour[2])*double(s.luminance));
		}
		tShaderOpAdd *add = new tShaderOpAdd();
		if (spec) {
			add->connect("spec",spec);
			delete spec;
		}
		if (diff) {
			add->connect("diff", diff);
			delete diff;
		}
		if (reflectSh) {
			add->connect("reflect", reflectSh);
			delete reflectSh;
		}
		if (refractSh) {
			add->connect("refract", refractSh);
			delete refractSh;
		}
		if (luminance) {
			add->connect("luminance", luminance);
			delete luminance;
		}
		sh->addShader(add);
		delete add;				//clear local copy
		edgeAngle.push_back((*sIt).maxSmoothingAngle);	//keep it for normal calc
		sIt++;
	}
    shape.setShader(sh);
	delete sh;

	//build vertex to polygon index vector (vertToPoly)
	std::vector<std::vector<unsigned int> > vertToPoly(shape.mVertexArray.size());
	for (unsigned int j=0; j<shape.mPolygonArray.size(); j++) {
		tPolygon &p = shape.mPolygonArray.at(j);
		std::vector<unsigned int>::iterator vi = p.vertexIndicies.begin();
		while(vi != p.vertexIndicies.end()) {
			vertToPoly.at((*vi)).push_back(j);	
			vi++;
		}//end polygon vertex index iterator
	}//end poly iterator

	//build vertex normal for each motherfuckin vertex in all polys
	std::vector<tPolygon>::iterator polyIt = shape.mPolygonArray.begin();
	unsigned int p=0;
	while (polyIt != shape.mPolygonArray.end()) {
		std::vector<unsigned int>::iterator vi = 
			(*polyIt).vertexIndicies.begin();
		vec3 pN, vN;

		//set polynormal
		pN = (*polyIt).polyNormal;
		
		while (vi != (*polyIt).vertexIndicies.end()) {
			//get polyNormal neighbour polys
			//vN = pN;	//set vertex normal to polynormal
            
			//iterate all neighbour polys
			std::vector<unsigned int>::iterator neighbPolyIndex = 
				vertToPoly.at((*vi)).begin();
			while (neighbPolyIndex != vertToPoly.at((*vi)).end()) {
				vec3 N;
				N = shape.mPolygonArray.at((*neighbPolyIndex)).polyNormal;
				double cosSmoothAng = 
					cos(edgeAngle.at(shape.mPolygonSurfaceIndex.at((*neighbPolyIndex))));
				if ( (N, pN) >= 
					cosSmoothAng || p == (*neighbPolyIndex) ) {
					//add this if smooth: dot > cos max smooth ang
					vN += N;
				}
				neighbPolyIndex++;
			}//end neighb polys
			vN.normalize();
			double4 vNorm;
			vNorm.x = vN[0]; vNorm.y = vN[1]; vNorm.z = vN[2]; vNorm.w = 1;
			(*polyIt).vertexNormals.push_back(vNorm);
			vN.set(0,0,0);
			vi++;
		}//end polys vertex indicies
		polyIt++;
		p++;
	}//end verttopoly iterator
	return 1;
}