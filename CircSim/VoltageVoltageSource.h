#pragma once
#include "Component.h"

class VoltageVoltageSource :
	public Component
{
public:
	VoltageVoltageSource();
	void setName(string name);
	~VoltageVoltageSource();
	void setValue(vector<string> line);
	void addStamp(double ** Matrix, unsigned variables);
};

