#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
#include "Netlist.h"
//TODO: deletar esse comentário
//TODO: testando revert

int main(int argc, char* argv[])
{


	//const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net";
	const char* netlistPath = "..\\..\\..\\simples.net";
	Netlist netlist(netlistPath);
	cout << "FEZ NETLIST" << endl;
	netlist.DoConductanceMatrixDC();
	cout << "FEZ A MATRIZ" << endl;
	netlist.SistemaLinear.PrintG_Matrix();
	netlist.SistemaLinear.SolveLinearSystem();
	cout << "RESOLVEU O SISTEMA" << endl;
	netlist.SistemaLinear.PrintG_Matrix();
	netlist.SistemaLinear.PrintVariables();
	system("pause");

	return 0;
}
