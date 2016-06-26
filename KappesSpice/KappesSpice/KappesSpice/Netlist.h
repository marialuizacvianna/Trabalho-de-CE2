#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"

using namespace std;

class Netlist
{
public:
	Netlist(string netlistPath);
private:
	vector <string> netlist;
	vector <Componente *> componentes;

};