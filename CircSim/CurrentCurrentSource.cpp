#include "stdafx.h"
#include "CurrentCurrentSource.h"
#include "SimulationParameters.h"


CurrentCurrentSource::CurrentCurrentSource()
{
	timeVariant = false;
	numberOfNodes = 4;
	numberOfExtraVariables = 1;
}

void CurrentCurrentSource::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("J_" + name);
}

CurrentCurrentSource::~CurrentCurrentSource()
{
}

void CurrentCurrentSource::setValue(vector<string> line)
{

	Value = stod(line[5]);
}


void CurrentCurrentSource::addStamp(double **Matrix, unsigned variables)
{
	if (nodes.at(0) != 0) {
		Matrix[nodes.at(0) - 1][extraVariablePosition[0] ] += Value;
	}
	if (nodes.at(1) != 0)
	{
		Matrix[nodes.at(1) - 1][extraVariablePosition[0]] -= Value;
	}
		if (nodes.at(2) != 0) {
		Matrix[extraVariablePosition[0]][nodes.at(2)-1] -= 1;
		Matrix[nodes.at(2) - 1][extraVariablePosition[0]] +=1;
	}
	if (nodes.at(3) != 0)
	{
		Matrix[extraVariablePosition[0]][nodes.at(3)-1] += 1;
		Matrix[nodes.at(3) - 1][extraVariablePosition[0]] -=1;
	}
}
