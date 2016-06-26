#include "stdafx.h"
#include "Switch.h"


Switch::Switch()
{
	numberOfNodes = 4;
	nonLinear = true;
}
void Switch::setValue(vector<string> line)
{
	
	if (line.size() > 5)
		this->Gon = stod(line[5]);
	if (line.size() > 6)
		this->Goff = stod(line[6]);
	if (line.size() > 7)
		this->tresholdVoltage = stod(line[7]);
}
void Switch::addStamp(double ** Matrix, vector<double> nodeVoltages, unsigned numberOfVariables)
{

	double voltageA = (nodes[2] == 0) ? 0 : nodeVoltages[nodes[2] - 1];
	double voltageB = (nodes[3] == 0) ? 0 : nodeVoltages[nodes[3] - 1];
	double G = ((voltageA - voltageB) >= tresholdVoltage) ? Gon : Goff;
	if (nodes.at(0) != 0)
		Matrix[nodes.at(0) - 1][nodes.at(0) - 1] += G;
	if (nodes.at(0) != 0 && nodes.at(1) != 0) {
		Matrix[nodes.at(0) - 1][nodes.at(1) - 1] += -G;
		Matrix[nodes.at(1) - 1][nodes.at(0) - 1] += -G;
	}
	if (nodes.at(1) != 0)
		Matrix[nodes.at(1) - 1][nodes.at(1) - 1] += G;
}


Switch::~Switch()
{
}
