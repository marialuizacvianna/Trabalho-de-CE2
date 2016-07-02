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
};

class Capacitor : public Componente
{ 
   public:
	void setInitialValue(double);
	double getInitialValue();
   private:
	double initialValue;
};

<<<<<<< HEAD
class Transformador : public Componente
{
private:
	double mIndutance; //indutancia que ele aplica no segundo indutor
};


=======
class Indutor :
	public Componente
	{
	public:
		void setInitialValue(double);
		double getInitialValue();
	private:
		double initialValue;
	};
>>>>>>> refs/remotes/origin/master
