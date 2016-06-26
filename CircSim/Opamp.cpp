#include "stdafx.h"
#include "Opamp.h"


Opamp::Opamp()
{
	numberOfExtraVariables = 1;
	numberOfNodes = 4;
}


Opamp::~Opamp()
{
}


void Opamp::setType(string a)
{
	Type = "O";
}

void Opamp::addStamp(double ** conductanceMatrix, unsigned numberOfVariables)
{
	if (nodes[0] != 0) {
		conductanceMatrix[nodes[0] - 1][extraVariablePosition[0]] = 1;
	}
	if (nodes[1] != 0)
	{
		conductanceMatrix[nodes[1] - 1][extraVariablePosition[0]] = -1;
	}
	if (nodes[2] != 0)
	{
		conductanceMatrix[extraVariablePosition[0]][nodes[2] - 1] = 1;
	}
	if (nodes[3] != 0)
	{
		conductanceMatrix[extraVariablePosition[0]][nodes[3] - 1] = -1;
	}
}

void Opamp::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("JO_" + name);
}