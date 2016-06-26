
#include "stdafx.h"
#include <iostream>
#include "Netlist.h"
#include "Solver.h"
#include "SimulationParameters.h"
#include <string>
//#include "cvplot.h"


int main(int argc, char* argv[])
{

	
	string netpath;

	cout << "Insira o caminho para a netlist" << endl;
	cin >> netpath;
	cin.ignore();

	Netlist netlist(netpath);
	Solver circuitSolver(netlist);
	circuitSolver.createConductanceMatrix();
	circuitSolver.printConductanceMatrix();
	circuitSolver.TimeSimulation();
	//circuitSolver.plotVariable(1);

	system("C:\\Python27\\python.exe plot.py");
	


	system("pause");
    return 0;
}

