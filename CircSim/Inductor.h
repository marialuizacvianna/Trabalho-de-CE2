#pragma once
#include "Component.h"
class Inductor :
	public Component
{
public:
	Inductor();
	virtual void addInitialCondition(vector<double> nodeVoltages);
	virtual void addStamp(double **, unsigned);
	virtual void postAnalisysCalculations(vector<double>);
	virtual void setName(string);
	~Inductor();
private:
	double lastCurrent;
	double stampValue;
	double sourceValue;
};

