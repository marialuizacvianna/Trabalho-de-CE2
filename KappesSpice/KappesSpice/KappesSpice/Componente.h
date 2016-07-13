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
	void addType(char);
	char getType();
	unsigned getNumberOfNodes();
	void SetExtraPosition(unsigned);
	unsigned GetExtraPosition(unsigned);
protected:
	char Type;
	vector<unsigned> nodes;
	unsigned numberOfNodes = 0;
	string Name;
	double Value;
	vector<unsigned> extraPosition;
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
    public:
		void setValueFirstIndutor(double);
		double getValueFirstIndutor(void);
		void setValueSecondIndutor(double);
		double getValueSecondIndutor(void);
		void setValueM(double);
		double getValueM();

    private:

	 double mIndutance; //indutancia que ele aplica no segundo indutor
	 double secondIndutorValue;
	 double firstIndutorValue;
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

class AmpOp :public Componente {
};

//parametro value e métodos getValue e setValue já definidos na classe Componente
class VoltageSrcCntrlVoltage : public Componente {
};

class CurrentSrcCntrlCurrent: public Componente {
};

class CurrentSrcCntrlVoltage: public Componente {
};

class VoltageSrcCntrlCurrent: public Componente {
};

class Mosfet: public Componente {
private:
	double gamma, //bulk threshold parameter
			phi, //surface potential
			lambda, //channel length modulation
			comprimento, largura,
			k; //transcondutance parameter

};
