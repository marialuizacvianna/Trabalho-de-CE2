#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
#include "LinearSystem.h"
#pragma package(smart_init)
#pragma hdrstop


#ifndef NETLIST_H
#define NETLIST_H

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

class Netlist  
{
public:
	Netlist(string netlistPath);
	void DoConductanceMatrixDC(void);
	void DoConductanceMatrixAC(void);
	void checkNewNode(unsigned);
	unsigned GetNumberOfNodes();
	void PrintNodes();
	LinearSystem SistemaLinear;
	string getTextoParaImprimirNaGui();
	char *textoParaImprimirNaGUI;
private:
	float frequency;
	vector<unsigned> netlistNodes;
	vector <string> netlist;
	vector <Componente *> componentes;
	//o compilador do Borland C++ builder não consegue acessar as funções stoul, stod e stof
	//por esse motivo, criei uma função chamada stoul, que invoca a função strtoul.
	//ao compilar no visual studio, basta comentar a declaração dessa função no netlist.cpp e no .h
	unsigned int stoul(string str);
	double stod(string str);
	float stof(string basic_string);
};
#ifdef __cplusplus
}
#endif
#endif