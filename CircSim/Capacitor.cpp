#include "stdafx.h"
#include "Capacitor.h"
#include <iostream>

Capacitor::Capacitor()
{
	voltageMemory.resize(4);
	currentMemory.resize(4);
}
void Capacitor::addInitialCondition(vector<double> nodeVoltages) {
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

void Capacitor::copyInitialConditions() {
	for (int i = 0; i < 4; i++)
	{

		voltageMemory[i] = 0;
		currentMemory[i] = 0;
	}
}


Capacitor::~Capacitor()
{
	cout << "Capacitor Criado porra";
	timeVariant = true;
	numberOfNodes = 2;
}

void Capacitor::addStamp(double ** conductanceMatrix, unsigned numberOfVariables)
{
	if (simParameters.adamsMethod == 1) {


		stampValue =  Value / (simParameters.stepSize / simParameters.subSteps);
		sourceValue = stampValue * voltageMemory[0];
		if (nodes.at(0) != 0)
		{
			conductanceMatrix[nodes.at(0) - 1][nodes.at(0) - 1] += stampValue;
			conductanceMatrix[nodes[0] - 1][numberOfVariables] += stampValue * voltageMemory[0];
		}
		if (nodes.at(0) != 0 && nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(0) - 1][nodes.at(1) - 1] += -stampValue;
			conductanceMatrix[nodes.at(1) - 1][nodes.at(0) - 1] += -stampValue;
		}
		if (nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(1) - 1][nodes.at(1) - 1] += stampValue;
			conductanceMatrix[nodes[1] - 1][numberOfVariables] -= stampValue * voltageMemory[0];
		}
	}
	else if (simParameters.adamsMethod == 2)
	{
		stampValue = 2 * Value / (simParameters.stepSize / simParameters.subSteps);
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
	else if (simParameters.adamsMethod == 3 )
	{
		stampValue = (12 * Value) /( 5 * (simParameters.stepSize / simParameters.subSteps));
		sourceValue = stampValue * voltageMemory[0] + ( (8.0/5.0)*currentMemory[0]- (currentMemory[1] / 5.0));
		if (nodes.at(0) != 0)
		{
			conductanceMatrix[nodes.at(0) - 1][nodes.at(0) - 1] += stampValue;
			conductanceMatrix[nodes[0] - 1][numberOfVariables] += sourceValue;
		}
		if (nodes.at(0) != 0 && nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(0) - 1][nodes.at(1) - 1] += -stampValue;
			conductanceMatrix[nodes.at(1) - 1][nodes.at(0) - 1] += -stampValue;
		}
		if (nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(1) - 1][nodes.at(1) - 1] += stampValue;
			conductanceMatrix[nodes[1] - 1][numberOfVariables] -= sourceValue;
		}
	}
	else if (simParameters.adamsMethod == 4 )
	{
		stampValue = 24 * Value / (9 * simParameters.stepSize / simParameters.subSteps);
		sourceValue = stampValue * voltageMemory[0] + 8 / 3.0 * (1 / 24.0 * currentMemory[2] - 5 / 24.0 * currentMemory[1] + 19 / 24.0 * currentMemory[0]);
		if (nodes.at(0) != 0)
		{
			conductanceMatrix[nodes.at(0) - 1][nodes.at(0) - 1] += stampValue;
			conductanceMatrix[nodes[0] - 1][numberOfVariables] += sourceValue;
		}
		if (nodes.at(0) != 0 && nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(0) - 1][nodes.at(1) - 1] += -stampValue;
			conductanceMatrix[nodes.at(1) - 1][nodes.at(0) - 1] += -stampValue;
		}
		if (nodes.at(1) != 0) {
			conductanceMatrix[nodes.at(1) - 1][nodes.at(1) - 1] += stampValue;
			conductanceMatrix[nodes[1] - 1][numberOfVariables] -= sourceValue;
		}
	}
}

void Capacitor::addIc(vector<string> line) {
	voltageMemory.resize(1);
	if (line.size() == 5) {
		voltageMemory[0] = (stod(line[4].substr(3, line[4].size() - 3)));
	}
	else {
		voltageMemory[0] = 0;
	}
}
void Capacitor::postAnalisysCalculations(vector<double> nodeVoltages)
{
	double voltageA = (nodes[0] == 0) ? 0 : nodeVoltages[nodes[0] - 1];
	double voltageB = (nodes[1] == 0) ? 0 : nodeVoltages[nodes[1] - 1];
	if (simParameters.adamsMethod == 1 || simParameters.actualStep == 0)
	{
		voltageMemory.insert(voltageMemory.begin(), voltageA - voltageB);
		currentMemory.insert(currentMemory.begin(), stampValue*(voltageMemory[0] - voltageMemory[1]));
	}
	else if (simParameters.adamsMethod == 2)
	{
		voltageMemory.insert(voltageMemory.begin(), voltageA - voltageB);
		currentMemory.insert(currentMemory.begin(), (voltageMemory[0] - voltageMemory[1])*stampValue - currentMemory[0]);
		voltageMemory.erase(--voltageMemory.end());
		currentMemory.erase(--currentMemory.end());
	}
	else if (simParameters.adamsMethod == 3)
	{
		voltageMemory.insert(voltageMemory.begin(), voltageA - voltageB);
		currentMemory.insert(currentMemory.begin(), stampValue * (voltageMemory[0] - voltageMemory[1]) - ((8.0/5.0) * currentMemory[0]) + (currentMemory[1]/5.0));
		voltageMemory.erase(--voltageMemory.end());
		currentMemory.erase(--currentMemory.end());
	}
	else if (simParameters.adamsMethod == 4)
	{
		voltageMemory.insert(voltageMemory.begin(), voltageA - voltageB);
		currentMemory.insert(currentMemory.begin(), stampValue * (voltageMemory[0] - voltageMemory[1]) - ((19.0 / 9.0) * currentMemory[0]) + ((5.0/9.0)*currentMemory[1]) - ((1.0/9.0)*currentMemory[2]));
		voltageMemory.erase(--voltageMemory.end());
		currentMemory.erase(--currentMemory.end());
	}
}
