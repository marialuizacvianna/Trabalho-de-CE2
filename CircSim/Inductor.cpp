#include "stdafx.h"
#include "Inductor.h"
#include <iostream>

Inductor::Inductor()
{
	numberOfNodes = 2;
	numberOfExtraVariables = 1;
	voltageMemory.push_back(0);
	currentMemory.push_back(0);
}

void Inductor::addInitialCondition(vector<double> nodeVoltages)
{
	double voltageA = (nodes[0] == 0) ? 0 : nodeVoltages[nodes[0] - 1];
	double voltageB = (nodes[1] == 0) ? 0 : nodeVoltages[nodes[1] - 1];
	voltageMemory.resize(4);
	currentMemory.resize(4);
	for (int i = 1; i < 4; i++)
	{

		voltageMemory[i] = voltageMemory[0];

		currentMemory[i] = currentMemory[0];
	}
}



Inductor::~Inductor()
{
}

void Inductor::addStamp(double ** conductanceMatrix, unsigned numberOfVariables)
{


	if (simParameters.adamsMethod == 1) {


		stampValue = Value / (simParameters.stepSize / simParameters.subSteps);
		sourceValue = stampValue * currentMemory[0];
		if (nodes.at(0) != 0) {
			conductanceMatrix[extraVariablePosition[0]][nodes.at(0) - 1] -= 1;
			conductanceMatrix[nodes.at(0) - 1][extraVariablePosition[0]] += 1;
		}
		if (nodes.at(1) != 0)
		{
			conductanceMatrix[extraVariablePosition[0]][nodes.at(1) - 1] += 1;
			conductanceMatrix[nodes.at(1) - 1][extraVariablePosition[0]] -= 1;
		}
		conductanceMatrix[extraVariablePosition[0]][numberOfVariables] += sourceValue;
		conductanceMatrix[extraVariablePosition[0]][extraVariablePosition[0]] += stampValue;

	}
	else if (simParameters.adamsMethod == 2)
	{
		stampValue = 2.0 * Value / (simParameters.stepSize / simParameters.subSteps);
		sourceValue = stampValue * currentMemory[0] + voltageMemory[0];
		if (nodes.at(0) != 0) {
			conductanceMatrix[extraVariablePosition[0]][nodes.at(0) - 1] -= 1;
			conductanceMatrix[nodes.at(0) - 1][extraVariablePosition[0]] += 1;
		}
		if (nodes.at(1) != 0)
		{
			conductanceMatrix[extraVariablePosition[0]][nodes.at(1) - 1] += 1;
			conductanceMatrix[nodes.at(1) - 1][extraVariablePosition[0]] -= 1;
		}
		conductanceMatrix[extraVariablePosition[0]][numberOfVariables] += sourceValue;
		conductanceMatrix[extraVariablePosition[0]][extraVariablePosition[0]] += stampValue;

	}
	else if (simParameters.adamsMethod == 3)
	{
		stampValue = 12 * Value / (5.0 * simParameters.stepSize / simParameters.subSteps);
		sourceValue = stampValue * currentMemory[0] + 12.0/5.0 * (2.0/3 * voltageMemory[0] - 1/12.0 * voltageMemory[1]);
		if (nodes.at(0) != 0) {
			conductanceMatrix[extraVariablePosition[0]][nodes.at(0) - 1] -= 1;
			conductanceMatrix[nodes.at(0) - 1][extraVariablePosition[0]] += 1;
		}
		if (nodes.at(1) != 0)
		{
			conductanceMatrix[extraVariablePosition[0]][nodes.at(1) - 1] += 1;
			conductanceMatrix[nodes.at(1) - 1][extraVariablePosition[0]] -= 1;
		}
		conductanceMatrix[extraVariablePosition[0]][numberOfVariables] += sourceValue;
		conductanceMatrix[extraVariablePosition[0]][extraVariablePosition[0]] += stampValue;

	}
	else if (simParameters.adamsMethod == 4 || simParameters.actualStep == 3)
	{
		stampValue = 3 * Value / (8 * simParameters.stepSize / simParameters.subSteps);
		if (nodes.at(0) != 0)
		{
			conductanceMatrix[nodes.at(0) - 1][nodes.at(0) - 1] += stampValue;
			conductanceMatrix[nodes[0] - 1][numberOfVariables] += stampValue * voltageMemory[0] + currentMemory[0];
		}
		if (nodes.at(0) != 0 && nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(0) - 1][nodes.at(1) - 1] += -stampValue;
			conductanceMatrix[nodes.at(1) - 1][nodes.at(0) - 1] += -stampValue;
		}
		if (nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(1) - 1][nodes.at(1) - 1] += stampValue;
			conductanceMatrix[nodes[1] - 1][numberOfVariables] -= stampValue * voltageMemory[0] + currentMemory[0];
		}
	}
}
void Inductor::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("J_" + name);
}
void Inductor::postAnalisysCalculations(vector<double> nodeVoltages)
{
	double voltageA = (nodes[0] == 0) ? 0 : nodeVoltages[nodes[0] - 1];
	double voltageB = (nodes[1] == 0) ? 0 : nodeVoltages[nodes[1] - 1];
	voltageMemory.insert(voltageMemory.begin(), voltageA - voltageB);
	currentMemory.insert(currentMemory.begin(), nodeVoltages[extraVariablePosition[0]]);
	voltageMemory.erase(--voltageMemory.end());
	currentMemory.erase(--currentMemory.end());
}
