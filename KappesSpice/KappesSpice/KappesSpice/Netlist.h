#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
#include "LinearSystem.h"

#define NR_TOLERANCE    1e-3
#define NR_ATTEMPTS  100
#define NR_RELATIVE_ABSOLUTE_TRESHOLD 1e-6

using namespace std;

class ACParameters
{
public:
	char stepType; // L for Linear; O for Octal; D for Decade;
	double points;
	double startFrequency;
	double endFrequency;
};

class Netlist  
{
public:
	Netlist(string netlistPath);
	void DoConductanceMatrixDC(void);
	void DoConductanceMatrixAC(void);
	void checkNewNode(unsigned);
	unsigned GetNumberOfNodes();
	void PrintNodes();
	int NewtonRaphson();
	void NewtonRaphsonError();
	void NewtonRaphsonRandomizeVariables();
	void NewtonRaphsonPrint();
	void ACSweep();
	void WriteDCData();
	void WriteACData();
	void WriteACLine();
	void CloseACFile();
	void PrintMosfetParameters();
	LinearSystem SistemaLinear;
	ACParameters ParametrosAC;
private:
	ofstream dcFile;
	ofstream acFile;
	string path;
	bool AC;  //define if it will be made the ACSweep
	double frequency;
	vector<unsigned> netlistNodes;
	vector <string> netlist;
	vector <Componente *> componentes;
	unsigned NewtonRaphsonIterations;
	bool convergiu;
};

