#include "stdafx.h"
#include "SimulationParameters.h"

using namespace std;

SimulationParameters simParameters;

void SimulationParameters::readFromNetlist(vector<string> line)
{
	simulationTime = stod(line[1]);
	stepSize = stod(line[2]);
	adamsMethod = line[3].back() - '0';
	subSteps = stoul(line[4]);
	newtonTolerance = 1e-9;
}
