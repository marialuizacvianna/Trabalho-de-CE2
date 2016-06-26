#include "stdafx.h"
#include "VoltageSource.h"
#include "SimulationParameters.h"
#include <iostream>

void VoltageSource::updateValue(unsigned iteration)
{
	if (voltageType == DC)
		return;
	if (voltageType == SIN)
	{
		double time = iteration * (simParameters.stepSize / simParameters.subSteps);
		Value = timeParameters[0] + timeParameters[1] * exp(-timeParameters[4] * (time - timeParameters[3])) * sin(2 * 3.141592 * timeParameters[2] * (time - timeParameters[3]) + 0.0175 * timeParameters[5]);
	}
	if (voltageType == PULSE)
	{

		double time = iteration * (simParameters.stepSize / simParameters.subSteps);
		double Vmin = timeParameters[0];
		double Vmax = timeParameters[1];
		double atraso = timeParameters[2];
		double subida = timeParameters[3];
		double descida = timeParameters[4];
		double ton = timeParameters[5];
		double periodo = timeParameters[6];
		unsigned periodoS = periodo * (simParameters.stepSize / simParameters.subSteps);
		double riseA = (Vmax - Vmin) / subida;
		double riseB = Vmin;
		double fallA = (Vmin - Vmax) / descida;
		double fallB = Vmax;
		double insidePeriodTime = (fmod(time + atraso, periodo));
		if (insidePeriodTime <= subida)
		{
			Value = riseA*insidePeriodTime + riseB;
		}
		else if (insidePeriodTime <= (subida + ton))
		{
			Value = Vmax;
		}
		else if (insidePeriodTime <= (subida + ton + descida))
		{
			Value = Vmax + fallA*(insidePeriodTime-subida-ton) ;
		}
		else {
			Value = Vmin;
		}

	}
}

VoltageSource::VoltageSource()
{
	timeVariant = true;
	numberOfNodes = 2;
	numberOfExtraVariables = 1;
}

void VoltageSource::setName(string name)
{
	Name = name;
	extraVariablesNames.push_back("J_" + name);
}

VoltageSource::~VoltageSource()
{
}

void VoltageSource::setValue(vector<string> line)
{

	if (line[3].compare("PULSE") == 0)
	{
		voltageType = PULSE;
		for (int i = 0; i < 7; i++)
			timeParameters.push_back(stod(line.at(i + 4)));
		
	}
	else if( line[3].compare("SIN") == 0)
	{
		voltageType = SIN;
		for (int i = 0; i < 7; i++)
			timeParameters.push_back(stod(line.at(i + 4)));
		Value = 5;
	}
	else if(line[3].compare("DC") == 0)
	{
		voltageType = DC;
		Value = stod(line[4]);
	}
	else {
		
	}
}


void VoltageSource::deleteStamp(double **Matrix, unsigned variables)
{
	if (nodes.at(0) != 0) {
		Matrix[extraVariablePosition[0]][nodes.at(0) - 1] += 1;
		Matrix[nodes.at(0) - 1][extraVariablePosition[0]] -= 1;
	}
	if (nodes.at(1) != 0)
	{
		Matrix[extraVariablePosition[0]][nodes.at(1) - 1] -= 1;
		Matrix[nodes.at(1) - 1][extraVariablePosition[0]] += 1;
	}
	Matrix[extraVariablePosition[0]][variables] += Value;
}

void VoltageSource::addStamp(double **Matrix, unsigned variables)
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
	Matrix[extraVariablePosition[0]][variables] -= Value;
}
