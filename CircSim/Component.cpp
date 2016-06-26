#include "stdafx.h"
#include "Component.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

 void Component::setName(string name)
{
	Name = name;
}

void Component::addNode(unsigned node)
{
	nodes.push_back(node);
}

string Component::getType()
{
	return Type;
}
void Component::setType(string t)
{
	Type = t;
}

void Component::addExtraNodePosition(unsigned p) {
	extraVariablePosition.push_back(p);
}
vector<string>& Component::getExtraVariablesNames()
{
	return extraVariablesNames;
}

unsigned Component::getNumberOfNodes()
{
	return numberOfNodes;
}

void Component::printNodes()
{
	for (int i = 0; i < numberOfNodes; i++);
		//cout << nodes[i] << " ";
}

string Component::getName()
{
	return Name;
}

unsigned Component::getNode(unsigned i)
{
	return nodes.at(i);
}

void Component::setValue(vector<string> line)
{
	Value = stod(line[3]);
}


unsigned Component::getNumberOfVariables()
{
	return numberOfExtraVariables;
}

bool Component::isTimeVariant()
{
	return timeVariant;
}

