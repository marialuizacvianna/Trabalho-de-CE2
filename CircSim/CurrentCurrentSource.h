#pragma once
#include "Component.h"

class CurrentCurrentSource :
	public Component
{
public:
	CurrentCurrentSource();
	void setName(string name);
	~CurrentCurrentSource();
	void setValue(vector<string> line);
	void addStamp(double ** Matrix, unsigned variables);
};

