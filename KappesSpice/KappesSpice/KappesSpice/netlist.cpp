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
			break;

<<<<<<< HEAD
		case 'I': // fonte de corrente independente
			string nome;
			CurrentSource *i = new CurrentSource;
			nome = lineParameters[0];
			nome.erase(nome.begin()); // remove the first letter, the component identifier
			(i->setName)(nome);
			(i->addNode)(stoul(lineParameters[1]));
			(i->addNode)(stoul(lineParameters[2]));
			(i->setValue)(stod(lineParameters[3]));
			componentes.push_back(i);
			break;

		case 'C': 
			string nome;
			Capacitor *c = new Capacitor;
			nome = lineParameters[0];
			nome.erase(nome.begin()); // remove the first letter, the component identifier
			(c->setName)(nome);
			(c->addNode)(stoul(lineParameters[1]));
			(c->addNode)(stoul(lineParameters[2]));
			(c->setValue)(stod(lineParameters[3]));
			(c->setInitialValue)(stod(lineParameters[4]));
			componentes.push_back(i);
			break;

		case 'K':
			string fistIndutor, secondIndutor;
			Tranformador *t = new Transformador;
			firstIndutor = lineParameters[1];
			secondIndutor = lineParameters[2];
			nome.erase(nome.begin()); // remove the first letter, the component identifier
			(c->setName)(nome);
			(c->addNode)(stoul(lineParameters[1]));
			(c->addNode)(stoul(lineParameters[2]));
			(c->setValue)(stod(lineParameters[3]));
			(c->setInitialValue)(stod(lineParameters[4]));
			componentes.push_back(i);
			break;
        
=======
			case 'L':
			string nome;
			Indutor *l = new Indutor;
			nome = lineParameters[0];
			nome.erase(nome.begin()); // remove the first letter, the component identifier
			(l->setName)(nome);
			(l->addNode)(stoul(lineParameters[1]));
			(l->addNode)(stoul(lineParameters[2]));
			(l->setValue)(stod(lineParameters[3]));
			(l->setInitialValue)(lineParameters[4]);
			componentes.push_back(l);
			break;

>>>>>>> refs/remotes/origin/master
		}



		index++;
	}
	netlistFile.close();

}


