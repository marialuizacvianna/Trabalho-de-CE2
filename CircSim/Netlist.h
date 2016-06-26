#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Component.h"

using namespace std;
class Netlist
{
public:
	Netlist(string filePath);
	vector<Component*>& getComponents();
	vector<unsigned>& getNodes();
	int getNumberOfComponents();
	~Netlist();
private:
	void readComponent(Component & component, vector<string> line);
	unsigned numberOfVariables = 0;
	vector<unsigned> _nodes;
	vector<Component*> _components;
	ifstream _netlistFile;
	
};

