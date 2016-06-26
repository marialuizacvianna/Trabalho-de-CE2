
#include "stdafx.h"
#include <iostream>
#include "Netlist.h"
#include "Solver.h"
#include "SimulationParameters.h"
#include <string>
#include <windows.h>

//#include "cvplot.h"


int main(int argc, char* argv[])
{



	string netpath;
	

	cout << "Insira o caminho para a netlist:";
	cin >> netpath;
	cin.ignore();
	try{
		Netlist netlist(netpath);
	
		size_t ponto = netpath.find(".");
		netpath.replace(ponto, netpath.length(), "-CS.tab");
		simParameters.outFile = netpath;
		Solver circuitSolver(netlist);
		circuitSolver.createConductanceMatrix();
		circuitSolver.TimeSimulation();
		cout << "Simulacao Finalizada" << endl;
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl ;
	}


	system("pause");
	return 0;
}

