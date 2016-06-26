#include "stdafx.h"
#include "VoltageCurrentSource.h"
#include "SimulationParameters.h"


VoltageCurrentSource::VoltageCurrentSource()
{
	timeVariant = false;
	numberOfNodes = 4;
	numberOfExtraVariables = 2;
}

void VoltageCurrentSource::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("Jx_" + name);
	extraVariablesNames.push_back("Jy_" + name);
}

VoltageCurrentSource::~VoltageCurrentSource()
{
}

void VoltageCurrentSource::setValue(vector<string> line)
{

	Value = stod(line[5]);
}


void VoltageCurrentSource::addStamp(double **Matrix, unsigned variables)
{
	if (nodes[0] != 0) {
		Matrix[nodes[0] - 1][extraVariablePosition[1]] += 1;
		Matrix[extraVariablePosition[1]][nodes[0] - 1] -= 1;
	}
	if (nodes[1] != 0) {
		Matrix[nodes[1] - 1][extraVariablePosition[1]] -= 1;
		Matrix[extraVariablePosition[1]][nodes[1] - 1] += 1;
	}
	if (nodes[2] != 0) {
		Matrix[nodes[2] - 1][extraVariablePosition[0]] += 1;
		Matrix[extraVariablePosition[0]][nodes[2] - 1] -= 1;
	}
	if (nodes[3] != 0) {
		Matrix[nodes[3] - 1][extraVariablePosition[0]] -= 1;
		Matrix[extraVariablePosition[0]][nodes[3] - 1] += 1;
	}
	Matrix[extraVariablePosition[1]][extraVariablePosition[0]] += Value;
}
