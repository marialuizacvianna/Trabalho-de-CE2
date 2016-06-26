#include "Diode.h"


Diode::Diode()
{
	nonLinear = true;
}

void Diode::setValue(vector<string> line)
{
	vector<double> x;
	vector<double> y;
	for (vector<string>::iterator v = (line.begin() + 3); v != line.end(); v++)
	{
		x.push_back(stod(*v));
		v++;
		y.push_back(stod(*v));
	}
	timeVariant = true;
}
Diode::~Diode()
{}
void Diode::addStamp(double **Matrix, vector<double>nodeVoltages, unsigned numberOfVariables)
{

	double voltageA = (nodes[0] == 0) ? 0 : nodeVoltages[nodes[0] - 1];
	double voltageB = (nodes[1] == 0) ? 0 : nodeVoltages[nodes[1] - 1];
	double v = voltageA - voltageB;
	double G = 1;
	double I = 1;
	if (nodes.at(0) != 0) {
		Matrix[nodes.at(0) - 1][nodes.at(0) - 1] += G;
		Matrix[nodes[0] - 1][numberOfVariables] += -I;
	}
	if (nodes.at(0) != 0 && nodes.at(1) != 0) {
		Matrix[nodes.at(0) - 1][nodes.at(1) - 1] += -G;
		Matrix[nodes.at(1) - 1][nodes.at(0) - 1] += -G;
	}
	if (nodes.at(1) != 0) {
		Matrix[nodes.at(1) - 1][nodes.at(1) - 1] += G;
		Matrix[nodes[1] - 1][numberOfVariables] += I;
	}
}
