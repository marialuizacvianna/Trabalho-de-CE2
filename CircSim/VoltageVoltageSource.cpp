#include "stdafx.h"
#include "VoltageVoltageSource.h"
#include "SimulationParameters.h"


VoltageVoltageSource::VoltageVoltageSource()
{
	timeVariant = false;
	numberOfNodes = 4;
	numberOfExtraVariables = 1;
}

void VoltageVoltageSource::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("J_" + name);
}

VoltageVoltageSource::~VoltageVoltageSource()
{
}

void VoltageVoltageSource::setValue(vector<string> line)
{

	Value = stod(line[5]);
}


void VoltageVoltageSource::addStamp(double **Matrix, unsigned variables)
{
	if (nodes.at(0) != 0) {
		Matrix[extraVariablePosition[0]][nodes.at(0) - 1] -= 1;
		Matrix[nodes.at(0) - 1][extraVariablePosition[0] ] += 1;
	}
	if (nodes.at(1) != 0)
	{
		Matrix[extraVariablePosition[0]][nodes.at(1) - 1] += 1;
		Matrix[nodes.at(1) - 1][extraVariablePosition[0]] -= 1;
	}
		if (nodes.at(2) != 0) {
		Matrix[extraVariablePosition[0]][nodes.at(2)-1] += Value;
	}
	if (nodes.at(3) != 0)
	{
		Matrix[extraVariablePosition[0]][nodes.at(3)-1] -= Value;
	}
}
