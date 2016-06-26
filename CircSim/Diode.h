#pragma once
#include "Component.h"
class Diode :
	public Component
{
public:
	Diode();
	virtual void setValue(vector<string> line);
	virtual void addStamp(double ** Matrix, vector<double> nodeVoltages, unsigned numberOfVariables);
	~Diode();
private:
	double Iss;
};

