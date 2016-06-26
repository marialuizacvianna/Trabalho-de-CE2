#pragma once
#include <string>
#include <vector>

using namespace std;
class SimulationParameters
{
public:
	double simulationTime;
	double stepSize;
	unsigned adamsMethod;
	unsigned subSteps;
	void readFromNetlist(vector<string> line);
	unsigned long actualStep = 0;
	double newtonTolerance = 1e-40;
	vector<unsigned> newtonNodes;
	unsigned nonLinearComponents = 0;
	unsigned newtonIteratio = 0;
	unsigned nonConvergingIterations = 0;
	string outFile;
};


extern SimulationParameters simParameters;