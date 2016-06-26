#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"

using namespace std;

void Componente::setName(string nome)
{
	Name = nome;
}

string Componente::getName()
{
	return Name;
}

void Componente::addNode(unsigned node)
{
	nodes.push_back(node);
}

unsigned Componente::getNode(unsigned i)
{
	return nodes.at(i);
}

void Componente::setValue(double valor)
{
	Value = valor;
}

double Componente::getValue()
{
	return Value;
}