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
	LinearSystem SistemaLinear;
private:
	vector <string> netlist;
	vector <Componente *> componentes;
};