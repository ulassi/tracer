#pragma once
/*
	This class is used to load scene files.

	Instanciates a new traceScene object for each valid scene file.

	TODO: implement this class, need a XML parser!
*/
#include <string>
#include "tScene.h"
#include <fstream>
class tShape;
class tLightSource;
extern std::ofstream gLog;

struct xmlNode {
	std::string				name;
	std::string				data;		//all data inside of the tag, if it has no children
	std::string				attributes;	//a string with all attributes
	std::vector<xmlNode>	children;
};


class tSceneLoader
{
public:
	tSceneLoader(void);
	~tSceneLoader(void);

	/*
		Tries to load file and appends all data to the supplied scene object.

		Returns
			0 if failed for some reason, info in logfile
			1 if scene is filled with the data from file specified
	*/
	int	load(std::string filename, tScene &scene);
private:
	bool parseDoc(xmlNode &root, std::string xmlfileData);
	bool parseChildren(xmlNode &parent, std::string &xml);
	
	xmlNode * findChild(xmlNode &parent, const char* byName);
	std::string findAttribute(xmlNode &node, const char* byName);
	bool parseAttributesAndTagName(xmlNode &node, std::string tagconent);

	//creator methods
	tShape *createShape(xmlNode &node);
	tLightSource *createLight(xmlNode &node);
	matrix4 createTransform(xmlNode &node);	
	tShader* createShader(xmlNode &node);
	tCamera *createCamera(xmlNode &node);
	void processCompositeShader(xmlNode &node, tShader *pshd);

	//cleans an XML string from comments 
	void cleanXMLString(std::string &xml);
	//removes spaces at beginning
	void trimString(std::string &xml);
};
