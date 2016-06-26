#include "stdafx.h"
#include "CurrentVoltageSource.h"
#include "SimulationParameters.h"


CurrentVoltageSource::CurrentVoltageSource()
{
	timeVariant = false;
	numberOfNodes = 4;
	numberOfExtraVariables = 0;
}

void CurrentVoltageSource::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("J_" + name);
}

CurrentVoltageSource::~CurrentVoltageSource()
{
}

void CurrentVoltageSource::setValue(vector<string> line)
{

	Value = stod(line[5]);
}


void CurrentVoltageSource::addStamp(double **Matrix, unsigned variables)
{
	
	if (nodes.at(2) != 0 && nodes.at(0) != 0 ) {
		Matrix[nodes.at(0) -1 ][nodes.at(2)-1] += Value;
	}
	if (nodes[1] != 0 && nodes[3] != 0)
	{
		Matrix[nodes.at(1) - 1][nodes.at(3) - 1] += Value;
	}
	if (nodes[0] != 0 && nodes[3] != 0)
	{
		Matrix[nodes.at(0) - 1][nodes.at(3) - 1] -= Value;
	}
	if (nodes[1] != 0 && nodes[2] != 0)
	{
		Matrix[nodes.at(1) - 1][nodes.at(2) - 1] -= Value;
	}
}
