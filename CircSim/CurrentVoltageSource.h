#pragma once
#include "Component.h"

class CurrentVoltageSource :
	public Component
{
public:
	CurrentVoltageSource();
	void setName(string name);
	~CurrentVoltageSource();
	void setValue(vector<string> line);
	void addStamp(double ** Matrix, unsigned variables);
};

