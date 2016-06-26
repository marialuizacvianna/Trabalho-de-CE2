#pragma once
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include "SimulationParameters.h"
using namespace std;

class Component
{
public:
	void setName(string);
	void addNode(unsigned);
	string getType();
	void setType(string);
	void addExtraNodePosition(unsigned p);
	vector<string> &getExtraVariablesNames();
	unsigned getNumberOfNodes();
	void printNodes();
	string getName();
	unsigned getNode(unsigned);
	void setValue(vector<string> line);
	unsigned getNumberOfVariables();
	virtual void addIc(vector<string> line) {};
	virtual void addStamp(double **, unsigned) {};
	virtual void addStamp(double **, vector<double>,unsigned numberOfVariables) {};
	virtual void updateValue(unsigned iteration) {};
	virtual void deleteStamp(double **, unsigned) {};
	virtual void postAnalisysCalculations(vector<double>) {};
	bool isTimeVariant();
	virtual void copyInitialConditions() {};
	virtual void addInitialCondition(vector<double> nodeVoltages) {};
	bool nonLinear = false;
protected:
	bool timeVariant = false;
	unsigned numberOfExtraVariables = 0;
	vector<double> voltageMemory;
	vector<double> currentMemory;
	vector<string> extraVariablesNames;
	vector<unsigned> extraVariablePosition;
	string Name;
	vector<unsigned> nodes;
	double ic;
	string Type;
	unsigned numberOfNodes = 2;
	double Value;
};

