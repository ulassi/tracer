#include "tshaderplugs.h"
#include "tShader.h"
tColor tShaderPlugs::evaluatePlug(const std::string &plugName, const tShaderData &data){
	std::map<std::string, tShader *>::iterator plug;
	plug = this->plugMap.find(plugName);
	if (plug != plugMap.end()) {
		//found something
		return (*plug).second->evaluate(data);
	}
	else {
		return tColor(0);
	}
}
tColor tShaderPlugs::transmitPlug(const std::string &plugName, const tShaderData &data){
	std::map<std::string, tShader *>::iterator plug;
	plug = this->plugMap.find(plugName);
	if (plug != plugMap.end()) {
		//found something
		return (*plug).second->transmit(data);
	}
	else {
		return tColor(0);
	}
}
void tShaderPlugs::addPlug(const std::string &name) {
	std::map<std::string, tShader *>::iterator plug;
	plug = this->plugMap.find(name);
	if (plug == plugMap.end()) {
		//found nothing, add
		this->plugMap.insert(std::pair<std::string, tShader*>(name, 0));		
	}
	
}

tShaderPlugs & tShaderPlugs::operator= (const tShaderPlugs& right) {
	/*
		Since we are storing a pointer as data, we
		cannot simpy do:
			this->plugMap = right.plugMap;
		We need to copy the pointer by hand, i.e. doing a
		deep copy (->clone())
	*/
	std::map<std::string, tShader*>::const_iterator mapIt = right.plugMap.begin();
	while (mapIt != right.plugMap.end()) {
		this->plugMap.insert(
			std::pair<std::string, tShader*>((*mapIt).first, (*mapIt).second->clone())
			);
		mapIt++;
	}
	return *this;
}

void tShaderPlugs::connect(const std::string &plugName, tShader *shader){
	std::map<std::string, tShader *>::iterator plug;
	plug = this->plugMap.find(plugName);
	if (plug != plugMap.end()) {
		//found something

		//Make sure we delete the copy already there dude
		if ((*plug).second != 0)
			delete (*plug).second;

		(*plug).second = shader->clone();
	}
	else {
		//add and connect again
		this->addPlug(plugName);
		this->connect(plugName, shader);
	}
}
tShaderPlugs::~tShaderPlugs(void) {
	/*
		Since we are using pointers, clear
		the map manually.
	*/
	std::map<std::string, tShader *>::iterator plug;
	plug = this->plugMap.begin();
	while (plug != this->plugMap.end()) {
		std::string name = (*plug).first;
		tShader *s = (*plug).second;
		(*plug).second = 0;
		if (s != 0)
			delete s;
		plug++;
	}
	plugMap.clear();
}
