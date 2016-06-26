#pragma once
#include "Component.h"
class Resistor :
	public Component
{
public:
	Resistor();
	virtual void addStamp(double ** Matrix, unsigned variables);
	~Resistor();
};

