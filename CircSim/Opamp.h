#pragma once
#include "Component.h"
class Opamp :
	public Component
{
public:
	Opamp();
	~Opamp();
	virtual void setName(string name);
	void setType(string a);
	virtual void addStamp(double **conductanceMatrix, unsigned numberOfVariables);
};

