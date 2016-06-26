#pragma once
#include "Component.h"
#include "SimulationParameters.h"
class Capacitor :
	public Component
{
public:
	Capacitor();
	virtual void addInitialCondition(vector<double> nodeVoltages);
	virtual void copyInitialConditions();
	~Capacitor();
	
	virtual void addStamp(double **, unsigned);
	void addIc(vector<string> line);
	virtual void postAnalisysCalculations(vector<double>);

private:
	double lastCurrent;
	double stampValue;
	double sourceValue;
};

