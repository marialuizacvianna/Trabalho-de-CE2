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

class Resistor :
	public Componente 
	{
	public:
		void printInfos();
};

class Indutor :
	public Componente
	{
	public:
		void setInitialValue(double);
		double getInitialValue();
	private:
		double initialValue;
	};
