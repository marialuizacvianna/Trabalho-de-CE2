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
	numberOfNodes += 1;
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

void Resistor::printInfos()
{
	cout << "Resistor :" << endl;
	cout << "Nome:\t" << Name << endl;
	cout << "Nodes:\t" << nodes[0] << "\t" << nodes[1] << endl;
	cout << "Valor:\t" << Value << endl;
	cout << endl;
}


void Capacitor::setInitialValue(double valorInicial)
{
	initialValue = valorInicial;
}

void Indutor::setInitialValue(double valorInicial)
{
	initialValue = valorInicial;
}

double Capacitor::getInitialValue()
{
	return initialValue;
}

double Indutor::getInitialValue()
{
	return initialValue;
}


void Transformador::setValueFirstIndutor(double valorPrimeiro)
{
	firstIndutorValue = valorPrimeiro;
}

double Transformador::getValueFirstIndutor()
{
	return firstIndutorValue;
}

void Transformador::setValueSecondIndutor(double valorSegundo)
{
	secondIndutorValue = valorSegundo;
}

double Transformador::getValueSecondIndutor()
{
	return secondIndutorValue;
}

void Transformador::setValueM(double indutanciaMutua)
{
	mIndutance = indutanciaMutua;

}

double Transformador:: getValueM()
{
	return mIndutance;
}

	void CurrentSource::setPhase(double fase){
		phase = fase;
	}
	
	double CurrentSource::getPhase(){
		return phase;
	}
	
	void CurrentSource::setDCValue(double valorDC){
		dcValue = valorDC;
	}
	
	double CurrentSource::getDCValue(){
		return dcValue;
	}

	void VoltageSource::setPhase(double fase){
				phase = fase;
	}
	
	double VoltageSource::getPhase(){
		return phase;
	}
	
	void VoltageSource::setDCValue(double valorDC){
		dcValue = valorDC;
	}
	
	double VoltageSource::getDCValue(){
		return dcValue;

	}


	void Componente::addType(char letra)
	{
		Type = letra;
    }

	char Componente::getType()
	{
		return Type;
	}

	unsigned Componente::getNumberOfNodes()
	{
		return numberOfNodes;
	}

	unsigned Componente::GetExtraPosition(unsigned indice)
	{
		return extraPosition[indice];
	}


	void Componente::SetExtraPosition(unsigned position)
	{
		extraPosition.push_back(position);
	}