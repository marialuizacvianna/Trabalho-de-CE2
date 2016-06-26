#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
#include "Netlist.h"

using namespace std;

Netlist::Netlist(string netlistPath)
{
	string linha;
	ifstream netlistFile;
	unsigned index = 0;

	netlistFile.open(netlistPath);
	if (!netlistFile)
	{
		cout << "Nao foi possivel abrir o arquivo" << endl;
	}
	cout << "Lendo Netlist : " << netlistPath << endl;
	getline(netlistFile, linha);
	while (getline(netlistFile, linha))
	{
		netlist.push_back(linha);
		cout << linha << endl;
		stringstream auxiliar(linha);   //initializing stringstream
		vector <string> lineParameters; // array that will hold the each parameter in the netlist line
		string campo;                   //auxiliar variable

		while (auxiliar.good()) //this while push back from the string stream each string in the line and record on the lineParameters array.
		{
			auxiliar >> campo;
			lineParameters.push_back(campo);
		}

		switch (linha.at(0)) //Get the first letter of the netlist, the identifier for each component
		{
		case 'R':
			string nome;
			Resistor *r = new Resistor;
			nome = lineParameters[0];
			nome.erase(nome.begin()); // remove the first letter, the component identifier
			(r->setName)(nome);
			(r->addNode)(stoul(lineParameters[1]));
			(r->addNode)(stoul(lineParameters[2]));
			(r->setValue)(stod(lineParameters[3]));
			componentes.push_back(r);
		}

		index++;
	}
	netlistFile.close();

}


