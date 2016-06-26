#pragma once
#include "Component.h"

class CurrentSource :
	public Component
{
public:
	CurrentSource();
	~CurrentSource();
	virtual void addStamp(double ** Matrix, unsigned variables);
};

