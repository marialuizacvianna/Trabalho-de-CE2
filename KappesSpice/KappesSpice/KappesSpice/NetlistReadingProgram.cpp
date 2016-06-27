#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
#include "Netlist.h"

int main(int argc, char* argv[])
{


	//const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net";
	const char* netlistPath = "..\\..\\..\\simples.net";
	Netlist netlist(netlistPath);

	system("pause");

	return 0;
}
