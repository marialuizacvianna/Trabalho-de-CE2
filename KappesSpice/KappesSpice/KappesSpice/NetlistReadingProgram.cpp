#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <complex>
#include "Componente.h"
#include "Netlist.h"

int main(int argc, char* argv[])
{

	//const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net";
	//const char* netlistPath = "..\\..\\..\\netlists de teste\\girador.net";

	const char* netlistPath = "..\\netlist\\";
	cout << "Insira o nome do arquivo a ser lido na pasta netlist. Exemplo <netlist.net>:" << endl;
	string arquivo;
	cin >> arquivo;
	arquivo = netlistPath + arquivo;
	cout << arquivo << endl;

	Netlist netlist(arquivo);
	cout << "FEZ NETLIST" << endl;
	netlist.SistemaLinear.setRowsValue(netlist.GetNumberOfNodes());
	netlist.SistemaLinear.InitializeG_Matrix();
	//netlist.DoConductanceMatrixDC();
	//cout << "FEZ A MATRIZ" << endl;
	//netlist.SistemaLinear.PrintG_Matrix();
	if (netlist.NewtonRaphson())
		return 1;
	cout << "Convergiu: RESOLVEU O SISTEMA" << endl;
	cout << endl;
	netlist.PrintMosfetParameters();
	//netlist.SistemaLinear.PrintG_Matrix();
	netlist.SistemaLinear.PrintVariables();
	netlist.WriteDCData();
	netlist.ACSweep();

	system("pause");

	return 0;
}
