#pragma once
#include <vector>
using namespace std;
class LinearNewtonRaphson
{
public:
	LinearNewtonRaphson(vector<double> x, vector<double> y);
	double getDerivativeAt(double x);
	double getFunctionAt(double x);
	double run(double initialValue);
	~LinearNewtonRaphson();
private:
	int findActualFunction(double x0);
	vector<double> bounds;
	vector<double> angularCoef;
	vector<double> linearCoef;
	double newtonTolerance = 0.000001;
};

