#include "stdafx.h"
#include ".\tsceneloader.h"
#include <iostream>
#include <fstream>

#include "tSphere.h"
#include "tInfinitePlane.h"
#include "tPolyShape.h"
#include "tPointLight.h"
#include "tAreaLight.h"
#include "tHemisphereLight.h"
#include "tShaderConstant.h"
#include "tShaderOpMult.h"
#include "tShaderOpAdd.h"
#include "tShaderDiffuse.h"
#include "tShaderPhongSpecular.h"
tSceneLoader::tSceneLoader(void)
{
}

tSceneLoader::~tSceneLoader(void)
{
}
/*
	Read scene file format. For XML schema see tracerscene.xsd		
*/
int	tSceneLoader::load(std::string filename, tScene &scene) {
	//TODO: break out XML parser module
	std::ifstream in(filename.c_str());
	std::string filestr, line;

	if (!in.is_open()) {
		gLog << ERRSTR << " scene file not found " << std::endl;
		return -1;
	}

	while(getline(in, line))
		filestr += line + " ";
	
	//parse document
	gLog << "parse scene : " << filename << std::endl;
	
	xmlNode scenenode;
	if (!parseDoc(scenenode, filestr)) {
		gLog << ERRSTR << " failed to parse xml document. " << std::endl;
		return 0;
	}
	//---------- Interpret xml tree

	//iterate children:
	std::vector<xmlNode>::iterator chIt = scenenode.children.begin();
	std::basic_string<char>::size_type npos = -1;
	while (chIt != scenenode.children.end()) {
		if ((*chIt).name == "shape") {
			tShape *shp = this->createShape((*chIt));
			if (shp) {
				scene.addShape(shp);
				delete shp;
			}
		}
		else if ((*chIt).name == "light") {
			tLightSource *li = this->createLight((*chIt));
			if (li) {
				scene.addLight(li);
				delete li;
			}
		}
		else if ((*chIt).name.find("camera") != npos) {
			tCamera *cam = this->createCamera((*chIt));
			if (cam) {
				scene.addCamera(cam);
				delete cam;
			}
		}
		chIt++;
	}
	return 1;
}
/*
	Takes:
		root		a reference to a xmlNode that is supposed to be
					the root of the document.
		xmlFileData	the string containing the whole xml file (string)
	Returns:
		bool		true for success, false for failure

	Remarks
		-Does not support nested tags of the same name:
			<tag1><tag1>data<tag1></tag1>
			
			Undefined behaviour when these circumstances occur.
			It is a bug, not a feature!
		-Non-validating parser
*/
bool tSceneLoader::parseDoc(xmlNode &root, std::string xmlfileData) {
	/*
		Search for xmlfileData
		find <anystr> and </anystr>
			set name of node to anystr
        
		start searching for children
	*/
	
	std::basic_string<char>::size_type  npos=-1, _stfirst=npos, _stsecond=npos, _endfirst; 
	std::string xml;
	
	//strip xml doc from any <?xml> tag
	xml = xmlfileData.substr(xmlfileData.find("?>")+2, xmlfileData.size());
	cleanXMLString(xml);

	//find root node
	_stfirst = xml.find_first_of('<');
	_endfirst = xml.find_first_of('>');
	if (_stfirst == npos || _endfirst == npos) {
		gLog << ERRSTR << " XML root tag not found " << std::endl;
		return false;
	}
	std::string tagcontent = xml.substr(_stfirst+1, _endfirst-2);
	//get name and attibutes
	parseAttributesAndTagName(root, tagcontent);
	std::string endtag = "</";
	printf("found ");printf(root.name.c_str());printf("\n");
	endtag += root.name;
	endtag += ">";
	_stsecond = xml.find(endtag);
	if (_stsecond == npos) {
		gLog << ERRSTR << " XML root tag not closed " << std::endl;
		return false;
	}
	xml = xml.substr(_endfirst + 1, _stsecond - _endfirst-1);
	
	bool result = parseChildren(root,xml); 
    return true;
};
/*
	Gets attributes and tag name from the string inide of
	the <> markers.
*/
bool tSceneLoader::parseAttributesAndTagName(xmlNode &node, std::string tagcontent) {
	std::basic_string<char>::size_type firstSpace = -1;
	firstSpace = tagcontent.find_first_of(' ');
	if (firstSpace == -1) { // no space
		node.name = tagcontent;
	}
	else {
		node.name = tagcontent.substr(0, firstSpace);	
		node.attributes = tagcontent.substr(firstSpace, tagcontent.size() - firstSpace-1);
	}
	return true;
}
/*
	Deletes all comment tags.
*/
void tSceneLoader::cleanXMLString(std::string &xml) {
	
	std::basic_string<char>::iterator xmlIt = xml.begin(), comIt;
	bool found_comment = false;
	while (xmlIt != xml.end()) {
		if ( (*(xmlIt)) == '<' && (*(xmlIt+1) )== '!' && (*(xmlIt +2) )== '-' && (*(xmlIt +3)) == '-') {
			//comment ALERT!
			comIt = xmlIt;

			//find end
			while (comIt != xml.end()) {
				if ( (*(comIt)) == '-' && (*(comIt+1)) == '-' && (*(comIt +2)) == '>' )  {
					found_comment = true;
					break;
				}
                comIt++;
			}
			if (found_comment) {
				//erase comment
				xml.erase(xmlIt, comIt+3);
				found_comment = false;
			}
		}
		
		xmlIt++;
	}
}
/*
	Finds child tags in xml and strips them from the string
*/
bool tSceneLoader::parseChildren(xmlNode &parent, std::string &xml) {
	/*
		while !endofstring(xml) 
			find <anystr> </anystr> pair, -> create node,
			call parseChildren(node, substring of xml)
	*/
	std::basic_string<char>::size_type npos = -1, startBeg = npos, startEnd = npos, closeBeg = npos, closeEnd = npos;
	std::string tagcontent, endtag;
	//Do until end of file
	while (true) {
		//will return npos if at end of file (xml will be empty)
		//find first tag in string
		startBeg = xml.find_first_of('<'); 
        startEnd = xml.find_first_of('>');
		
		if (startBeg == npos || startEnd == npos) {
			//no tags left, just data (if any)
			parent.data = xml;
			trimString(parent.data);
			return false;
		}
		
		//get string between start and end tags
		xmlNode child;
		tagcontent = xml.substr(startBeg+1, startEnd - startBeg-1);	
		parseAttributesAndTagName(child, tagcontent);				//strip attributes and tagname
		endtag = "</"; 
		endtag += child.name + ">";
		closeBeg = xml.find(endtag);
		closeEnd = closeBeg + endtag.size();
		if (closeBeg == npos) {
			gLog << ERRSTR << " XML tag not closed " << std::endl;
			return false;
		}
		printf("found ");printf(child.name.c_str());printf("\n");
		std::string data = xml.substr(startEnd + 1, closeBeg - startEnd-1);
		
		//parse child data
		this->parseChildren(child, data);
		parent.children.push_back(child);
		xml.erase(startBeg, closeEnd - startBeg);
	}
	return true;
}
/*
	Removes \n \t in string
*/
void tSceneLoader::trimString(std::string &xml) {
	std::basic_string<char>::iterator strIt = xml.begin();
	while (strIt != xml.end()) {
		if ((*strIt) != '\t' && (*strIt) != '\n' && (*strIt) != ' ') {
			break;
		}
		xml.erase(strIt);
	}
}
/*
	Utility function to get a child by certain name.
*/
xmlNode * tSceneLoader::findChild(xmlNode &parent, const char* byName){
	std::vector<xmlNode>::const_iterator chIt = parent.children.begin();
	while (chIt != parent.children.end()) {
		if ((*chIt).name == byName) {
			return (xmlNode*) &(*chIt);
		}
		chIt++;
	}
	return 0;
}
std::string tSceneLoader::findAttribute(xmlNode &node, const char* byName) {
	std::basic_string<char>::size_type npos=-1,attpos=npos;
	std::string result("");
	attpos = node.attributes.find(std::string(byName));
	if (attpos != npos) {
		std::basic_string<char>::size_type strBeg=npos, strEnd=npos;
		std::string tmp;
		tmp = node.attributes.substr(attpos);
		strBeg = tmp.find("\"");
		strEnd = tmp.find("\"", 2);
		if (strEnd != npos && strBeg != npos) {
			//set result to string inside "" of attribute
			result = tmp.substr(strBeg+1, tmp.size() - strEnd-1);
		}
	}
	return result;
}
tCamera *tSceneLoader::createCamera(xmlNode &node) {
	xmlNode *eyep = findChild(node, "eyepoint");
	xmlNode *ori = findChild(node, "orientation");
	xmlNode *aspect = findChild(node, "aspectratio");
	xmlNode *implanedist = findChild(node, "imageplanedistance");
	xmlNode *fov = findChild(node, "fov");
	tCamera *cam = new tCamera();
	if (eyep) {
		float x,y,z;
		sscanf(eyep->data.c_str(),"%f %f %f", &x, &y, &z);
		cam->setEyePoint(vec3(x,y,z));
	}
	if (ori) {
		float x,y,z;
		sscanf(ori->data.c_str(),"%f %f %f", &x, &y, &z);
		cam->setOrientation(x,y,z);
	}
	if (aspect) {
		float f;
		sscanf(aspect->data.c_str(), "%f", &f);
		cam->setAspectRatio(f);
	}
	if (implanedist) {
		float f;
		sscanf(implanedist->data.c_str(), "%f", &f);
		cam->setDistanceToImagePlane(f);
	}
	if (fov) {
		float f;
		sscanf(fov->data.c_str(), "%f", &f);
		cam->setFieldOfView(f);
	}
	return cam;
}
/*
	Assumes node containing shape data
*/
tShape* tSceneLoader::createShape(xmlNode &node) {
	tShape *retshp=0;
	xmlNode *tfNode = findChild(node, "transform");
	//create tf
	matrix4 tf;
	if (tfNode)
		tf = createTransform(*tfNode);
		
	xmlNode *geoNode = findChild(node, "geometry");
	//create shape
	if (geoNode) {
		xmlNode *sphNode = findChild(*geoNode, "sphere");
		xmlNode *planeNode = findChild(*geoNode, "plane");
		xmlNode *polyNode = findChild(*geoNode, "polyshape");
		if (sphNode) {
			tSphere *sph = new tSphere();
			sph->transform = tf;
			xmlNode *radii = findChild(*sphNode, "radius");
			if (radii) {
				float r;
				sscanf(radii->data.c_str(), "%f", &r);
				sph->r = r;
			}
			retshp = sph;
		}
		else if (planeNode) {
			tInfinitePlane *pl = new tInfinitePlane();
			pl->transform = tf;
			xmlNode *vals = findChild(*planeNode, "abcd");
			if (vals) {
				float a,b,c,d;
				sscanf(vals->data.c_str(),"%f %f %f %f", &a, &b, &c, &d);
				pl->a = a;
				pl->b = b;
				pl->c = c;
				pl->d = d;
			}
			retshp = pl;
		}
		else if (polyNode) {
			//TODO: support polyshape in scene file
		}
	}
	xmlNode *sh_node = findChild(node, "shader");
	tShader *shd = 0;
	if (sh_node) {
		//TODO: handle all shadernodes, pass them to the shader factory		
		xmlNode *child = 0;
		if (sh_node->children.size() > 0) {
			child = &sh_node->children.at(0);
			shd = createShader(*child);
		}
		else 
			shd = new tShaderConstant();
		retshp->setShader(shd);
		delete shd;
	}
	else { 
		//default to a constant-color
		tShaderConstant *shconst = new tShaderConstant();
		shd = shconst;
		if (retshp)
			retshp->setShader(shd);
		delete shconst;
	}
	
	return retshp;
}
/*
	Creates a shader based on the data inside the child,
	or depending on its name.
*/
tShader* tSceneLoader::createShader(xmlNode &node) {
	tShader *shd=0;
	//iterate all children
	std::basic_string<char>::size_type npos = -1;
	
	//std::string nodename = this->findAttribute(*node, "name");
	if (node.name.find("shaderoperator") != npos) {
		//create op
		if (node.data.find("multiply") != npos) 
		{
			//op *
			tShader *op = new tShaderOpMult();
			processCompositeShader(node, op);
			return op;
		}
		else if (node.data.find("add")  != npos) 
		{
			//op +
			tShader *op = new tShaderOpAdd();
			processCompositeShader(node, op);
			return op;
		}
	}
	else if (node.name.find("shaderinput") != npos){
		if (node.data.find("diff") != npos) {
			//std diffuse shader
			shd = new tShaderDiffuse();
			processCompositeShader(node, shd);
			return shd;
		}
		else if (node.data.find("spec") != npos) {
			//std specular
			shd = new tShaderPhongSpecular();
			processCompositeShader(node, shd);
			return shd;
		}
	}
	else if (node.name.find("color") != npos ){
		float r,g,b;
		sscanf(node.data.c_str(), "%f %f %f", &r, &g, &b);
		tShader *shd = new tShaderConstant(tColor(r,g,b));
		return shd;
	}
	else if (node.name.find("const") != npos ){
		float c;
		sscanf(node.data.c_str(), "%f", &c);
		tShader *shd = new tShaderConstant(tColor(c));
		return shd;
	}
	//TODO: Add all shaders here dude
	
	return 0;
}
/*
	Connect plugs (children) to a shader node, parentNode
*/
void tSceneLoader::processCompositeShader(xmlNode & parentNode, tShader *parentShader) {
	std::vector<xmlNode>::iterator childIt =
		parentNode.children.begin();
	while (childIt != parentNode.children.end()) {
		std::string plugName = findAttribute((*childIt), "name");
        tShader *shd = createShader((*childIt));
		parentShader->connect(plugName, shd);
		delete shd;
		childIt++;
	}
}
/*
	Spawns a lightsource from a xmlNode
*/
tLightSource* tSceneLoader::createLight(xmlNode &node) {
	tLightSource *retli = 0;
	matrix4 tf;
	int samples = 1;
    xmlNode *lightshape = findChild(node, "lightshape");
	xmlNode *transform = findChild(node, "transform");
	xmlNode *sampleNode = findChild(node, "multisample");
	if (transform) 
		tf = createTransform(*transform);
	if (sampleNode) {
		sscanf(sampleNode->data.c_str(), "%d", &samples);
	}
	if (lightshape) {
		xmlNode *point = findChild(*lightshape, "pointlight");
		xmlNode *area = findChild(*lightshape, "arealight");
		xmlNode *hemi = findChild(*lightshape, "hemisphere");
		if (point) {
			tPointLight *pli = new tPointLight();
			xmlNode *color = findChild(*point, "lightcolor");
			if (color) {
				float r,g,b;
				sscanf(color->data.c_str(), "%f %f %f", &r, &g, &b);
				pli->color = tColor(r,g,b);
			}
			pli->position = tf.getTranslation();
			retli = pli;
		}
		else if(area) {
			tAreaLight *ali = new tAreaLight();
			xmlNode *color = findChild(*area, "lightcolor");
			xmlNode *up = findChild(*area, "upaxis");
			xmlNode *right = findChild(*area, "rightaxis");
			xmlNode *sizeUp = findChild(*area, "sizeup");
			xmlNode *sizeRight = findChild(*area, "sizeright");
			vec3 rVec(0,0,1), uVec(0,1,0);
			double sizeR=1, sizeU=1;
			if (color) {
				float r,g,b;
				sscanf(color->data.c_str(), "%f %f %f", &r, &g, &b);
				ali->lightCol = tColor(r,g,b);
			}
			if (up) {
				float x,y,z;
				sscanf(up->data.c_str(), "%f %f %f", &x, &y, &z);
				uVec.set(x,y,z);
			}
			if (right) {
				float x,y,z;
				sscanf(right->data.c_str(), "%f %f %f", &x, &y, &z);
				rVec.set(x,y,z);
			}
			if (sizeUp) {
				float r;
				sscanf(sizeUp->data.c_str(), "%f", &r);
				sizeU = r;
			}
			if (sizeRight) {
				float r;
				sscanf(sizeRight->data.c_str(), "%f", &r);
				sizeR = r;
			}
			ali->setSpanAndPlane(rVec, uVec, tf.getTranslation(), sizeR, sizeU);
			ali->setNumberOfSamples(samples);
			retli = ali;
		}
		else if (hemi) {
			tHemisphereLight *hli = new tHemisphereLight();
			hli->setNumberOfSamples(samples);
			xmlNode *color = findChild(*area, "lightcolor");
			if (color) {
				float r,g,b;
				sscanf(color->data.c_str(), "%f %f %f", &r, &g, &b);
				hli->constantColor = tColor(r,g,b);
			}
			retli = hli;
		}
	}
	return retli;
}
/*
	Translates a <transform> node into a matrix4 transform matrix
*/
matrix4 tSceneLoader::createTransform(xmlNode &node) {
	matrix4 mat;
	matrix3 rotMat;
	vec3 transVec;
	xmlNode *transl = findChild(node, "translation");
	xmlNode *rot = findChild(node, "rotation");
	if (transl) {
		float x,y,z;
		sscanf(transl->data.c_str(),"%f %f %f", &x, &y, &z);
		transVec.set(x,y,z);
	}
	if (rot) {
		xmlNode *axis = findChild(*rot, "axis");
		xmlNode *angle = findChild(*rot, "angle");
		if (axis && angle && !axis->data.empty() && !angle->data.empty()) {
			float vx,vy,vz,ang;
			sscanf(axis->data.c_str(), "%f %f %f", &vx, &vy, &vz);
			sscanf(angle->data.c_str(), "%f", &ang);
			vec3 axisVec(vx, vy, vz);
			rotMat.setRotation(axisVec, ang);
		}
	}
	mat.setTransformation(transVec, rotMat);
	return mat;
}
