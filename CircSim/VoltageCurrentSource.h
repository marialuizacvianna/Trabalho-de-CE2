#pragma once
#include "Component.h"

class VoltageCurrentSource :
	public Component
{
public:
	VoltageCurrentSource();
	void setName(string name);
	~VoltageCurrentSource();
	void setValue(vector<string> line);
	void addStamp(double ** Matrix, unsigned variables);
};

