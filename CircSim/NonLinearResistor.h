#pragma once
#include "Component.h"
#include "LinearNewtonRaphson.h"
class NonLinearResistor :
	public Component
{
public:
	NonLinearResistor();
	virtual void setValue(vector<string> line);
	void addStamp(double ** Matrix, vector<double>,unsigned);
	~NonLinearResistor();
private:
	LinearNewtonRaphson *newtonRaphson;
};

