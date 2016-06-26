#include "stdafx.h"
#include "LinearNewtonRaphson.h"
#include "SimulationParameters.h"

LinearNewtonRaphson::LinearNewtonRaphson(vector<double> x, vector<double> y)
{
	bounds = x;
	for (int i = 1; i < x.size(); i++)
	{
		angularCoef.push_back((y[i] - y[i - 1]) / (x[i] - x[i - 1]));
		linearCoef.push_back(-(*(angularCoef.end()-1)) * x[i] + y[i]);
	}
}


double LinearNewtonRaphson::getDerivativeAt(double x){
	int function = findActualFunction(x);
	return angularCoef[function];
}

double LinearNewtonRaphson::getFunctionAt(double x) {
	int function = findActualFunction(x);
	return angularCoef[function] * x + linearCoef[function];
}

double LinearNewtonRaphson::run(double initialValue)
{
	double value;
	double lastValue = initialValue;
	int function = findActualFunction(lastValue);
	
	value = lastValue - (angularCoef[function] * lastValue + linearCoef[function]) / angularCoef[function];
	return initialValue;
}


LinearNewtonRaphson::~LinearNewtonRaphson()
{
}

int LinearNewtonRaphson::findActualFunction(double x0)
{
	if (x0 < bounds[0]) {
		return 0;
	}
	else if (x0 < bounds[1]) {
		return 0;
	}
	else if(x0 < bounds[2]){
		return 1;
	}
	return 2;
}
