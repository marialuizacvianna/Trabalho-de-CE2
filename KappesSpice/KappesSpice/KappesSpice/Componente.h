#pragma once
#include <string>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;

class Componente
{
public:
	void setName(string);
	string getName();
	void addNode(unsigned);
	unsigned getNode(unsigned);
	void setValue(double);
	double getValue();
protected:
	string Name;
	vector<unsigned> nodes;
	unsigned numberOfNodes = 2;
	double Value;
};
