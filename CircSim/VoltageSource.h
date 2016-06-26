#pragma once
#include "Component.h"

enum VoltageSourceType {DC,SIN,PULSE};
class VoltageSource :
	public Component
{
public:
	void updateValue(unsigned iteration);
	VoltageSource();
	void setName(string name);
	~VoltageSource();
	void setValue(vector<string> line);
	void deleteStamp(double ** Matrix, unsigned variables);
	void addStamp(double ** Matrix, unsigned variables);
private:
	vector<double> timeParameters;
	VoltageSourceType voltageType;
};

