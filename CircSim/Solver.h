#pragma once
#include "Netlist.h"
#include <vector>
#include <string>
#include <fstream>

class Solver
{
public:
	bool calculateNewtonRaphson();
	vector<unsigned> getNonConvergingVariables();
	void TimeSimulation();
	void outputToFile();
	void addStamps();
	//void plotVariable(unsigned i);
	void createAdjacenceMatrix();
	void createConductanceMatrix();
	void printConductanceMatrix();
	void SolveConductanceMatrix();
	void printResults();
	void computeOpamp(Component *opamp);
	Solver(Netlist &Net);
	~Solver();
private:
	ofstream *outputFile;
	Netlist &net;
	void clearCondunctanceMatrix();
	unsigned numberOfVariables;
	char ** _adjacentMatrix;
	double ** _conductanceMatrix;
	vector<Component*> _components;
	vector<unsigned> _nodes;
	vector<unsigned> newtonRaphsonNodes;
	vector<double> NonLinearComponenetVoltages;
	vector<double> lastNonLinearComponenetVoltages;
	vector<string> _nodeNames;
	vector<double> x;
	vector<double> lastX;
};