#include "stdafx.h"
#include "Resistor.h"



Resistor::Resistor()
{
	numberOfNodes = 2;
}

void Resistor::addStamp(double **Matrix,unsigned variables)
{
	if(nodes.at(0) != 0)
		Matrix[nodes.at(0)-1][nodes.at(0)-1] += 1 / Value;
	if (nodes.at(0) != 0 && nodes.at(1) != 0) {
		Matrix[nodes.at(0) -1][nodes.at(1) -1] += -1 / Value;
		Matrix[nodes.at(1) -1][nodes.at(0) -1] += -1 / Value;
	}
	if(nodes.at(1) != 0)
		Matrix[nodes.at(1) -1][nodes.at(1) -1] += 1 / Value;
}

Resistor::~Resistor()
{
}
