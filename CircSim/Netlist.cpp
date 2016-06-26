#include "stdafx.h"
#include "Netlist.h"
#include "Resistor.h"
#include "CurrentSource.h"
#include "VoltageSource.h"
#include "SimulationParameters.h"
#include "Opamp.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "NonLinearResistor.h"
#include "VoltageVoltageSource.h"
#include "CurrentCurrentSource.h"
#include "CurrentVoltageSource.h"
#include "VoltageCurrentSource.h"
#include "Switch.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>


using namespace std;

void Netlist::readComponent(Component &component,vector<string> line)
{
	unsigned node;
	unsigned i;
	for ( i = 1; i < component.getNumberOfNodes() + 1; i++)
	{
		node = stoul(line[i]);
		if (!binary_search(_nodes.begin(), _nodes.end(), node))
		{
#ifdef DEBUG
			cout << endl << "Novo No Adicionado No " << node << endl;
#endif			//Insere em ordem
			vector<unsigned>::iterator it = lower_bound(_nodes.begin(), _nodes.end(), node);
			_nodes.insert(it, node);
		}
		component.addNode(node);
	}
	component.printNodes();

}

Netlist::Netlist(string filePath)
{
	_netlistFile.open(filePath);
	if (!_netlistFile)
	{
		throw runtime_error("Ocorreu Um Erro Ao abrir Seu arquivo");
	}
	string line;
	int i = 0;
	getline(_netlistFile, line); // Pula Primeira Linha
	while (getline(_netlistFile, line))
	{
		vector<string> lineArr;
		int i = 0;
		stringstream ssin(line);
		string word;
		while (ssin.good()) {
			ssin >> word;
			lineArr.push_back(word);
			++i;
		}
		if (line.at(0) == 'R')
		{
			Resistor *r = new Resistor();
			r->setValue(lineArr);
			_components.push_back(r);
			_components.back()->setName(lineArr[0]);
			readComponent(*_components.back(), lineArr);
		}
		if (line.at(0) == 'C')
		{
			Capacitor *r = new Capacitor();
			r->setValue(lineArr);
			r->addIc(lineArr);
			_components.push_back(r);
			_components.back()->setName(lineArr[0]);
			readComponent(*_components.back(), lineArr);
		}
		if (line.at(0) == 'O')
		{
			Opamp *op = new Opamp();
			op->setType("O");
			op->setName("O");
			_components.push_back(op);
			_components.back()->setName(lineArr[0]);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'I')
		{
			CurrentSource *i = new CurrentSource();
			i->setValue(lineArr);
			_components.push_back(i);	
			_components.back()->setName(lineArr[0]);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'V')
		{
			VoltageSource *v = new VoltageSource();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'L')
		{
			Inductor *v = new Inductor();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'N')
		{
			NonLinearResistor *v = new NonLinearResistor();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			simParameters.newtonNodes.push_back(stoul(lineArr[1]));
			simParameters.newtonNodes.push_back(stoul(lineArr[2]));
			simParameters.nonLinearComponents++;
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'G')
		{
			CurrentVoltageSource *v = new CurrentVoltageSource();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'E')
		{
			VoltageVoltageSource *v = new VoltageVoltageSource();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == 'F')
		{
			CurrentCurrentSource *v = new CurrentCurrentSource();
			v->setName(lineArr[0]);
			v->setValue(lineArr);
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == '$')
		{
			Switch *v = new Switch();
			v->setName(lineArr[0]);
			v->setValue(lineArr);;
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
			simParameters.newtonNodes.push_back(stoul(lineArr[1]));
			simParameters.newtonNodes.push_back(stoul(lineArr[2]));
			simParameters.newtonNodes.push_back(stoul(lineArr[3]));
			simParameters.newtonNodes.push_back(stoul(lineArr[4]));
		}
		else if (line.at(0) == 'H')
		{
			VoltageCurrentSource *v = new VoltageCurrentSource();
			v->setName(lineArr[0]);
			v->setValue(lineArr);;
			_components.push_back(v);
			readComponent(*_components.back(), lineArr);
		}
		else if (line.at(0) == '.')
		{
			simParameters.readFromNetlist(lineArr);
		}
	}
}

vector<Component*>& Netlist::getComponents()
{
	return _components;
}

vector<unsigned>& Netlist::getNodes()
{
	return _nodes;
}

int Netlist::getNumberOfComponents()
{
	return 0;
}

Netlist::~Netlist()
{
	_netlistFile.close();
}
