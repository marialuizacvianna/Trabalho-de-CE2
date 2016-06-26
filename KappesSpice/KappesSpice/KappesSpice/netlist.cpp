#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"

using namespace std;

int main(int argc, char* argv[])
{
 string linha;

 const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net" ;
 vector <string> netlist;
 vector <Componente *> componentes;
 ifstream netlistFile;
 unsigned index = 0;

 netlistFile.open(netlistPath);
 if(!netlistFile)
 {
 	cout << "Nao foi possivel abrir o arquivo" << endl;
 }
 cout << "Lendo Netlist : " << netlistPath <<endl ;
 getline(netlistFile, linha) ;
 while(getline(netlistFile, linha))
 {
 netlist.push_back(linha) ;
 cout << linha << endl ;
 stringstream auxiliar(linha);   //initializing stringstream
 vector <string> lineParameters; // array that will hold the each parameter in the netlist line
 string campo;                   //auxiliar variable

 while(auxiliar.good()) //this while push back from the string stream each string in the line and record on the lineParameters array.
 {
  auxiliar >> campo;
  lineParameters.push_back(campo);
 }

 switch (linha.at(0)) //Get the first letter of the netlist
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

 system("pause");

 return 0;
}
