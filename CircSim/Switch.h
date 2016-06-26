#pragma once
#include "Component.h"
class Switch :
	public Component
{
public:
	Switch();
	virtual void setValue(vector<string> line);
	virtual void addStamp(double ** Matrix, vector<double> nodeVoltages, unsigned numberOfVariables);

	~Switch();
private:
	double tresholdVoltage = 1;
	double Gon = 1e-20;
	double Goff = 1e20;
};

