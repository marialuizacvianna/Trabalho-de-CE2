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

using namespace std;

class Netlist  
{
public:
	Netlist(string netlistPath);
	void DoConductanceMatrix(void);
	void checkNewNode(unsigned);
	unsigned GetNumberOfNodes();
	void PrintNodes();
	LinearSystem SistemaLinear;
private:
	vector<unsigned> netlistNodes;
	vector <string> netlist;
	vector <Componente *> componentes;
};