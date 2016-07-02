#pragma once
#include <string>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;

class Componente
{
public:
	void setName(string);
	string getName();
	void addNode(unsigned);
	unsigned getNode(unsigned);
	void setValue(double);
	double getValue();
protected:
	string Name;
	vector<unsigned> nodes;
	unsigned numberOfNodes;
	double Value;
};

class Resistor : public Componente 
{
	public:
    	void printInfos();
};

class CurrentSource: public Componente
{
	private:
	double phase;
	double dcValue;
public:
	void setPhase(double);
	double getPhase();
	void setDCValue(double);
	double getDCValue();
};

class VoltageSource: public Componente
{
private:
	double phase;
	double dcValue;
public:
	void setPhase(double);
	double getPhase();
	void setDCValue(double);
	double getDCValue();	
};

class Capacitor : public Componente
{ 
   public:
	void setInitialValue(double);
	double getInitialValue();
   private:
	double initialValue;
};

class Transformador : public Componente
{
private:
	double mIndutance; //indutancia que ele aplica no segundo indutor
};


class Indutor :
	public Componente
	{
	public:
		void setInitialValue(double);
		double getInitialValue();
	private:
		double initialValue;
	};
