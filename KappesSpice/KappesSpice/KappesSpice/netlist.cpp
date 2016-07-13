#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "LinearSystem.h"
#include "Componente.h"
#include "Netlist.h"

using namespace std;

Netlist::Netlist(string netlistPath)
{
	string linha;
	ifstream netlistFile;
	unsigned index = 0;
   
	extraRows = 0;

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
		{
			string nomeR;
			Resistor *r = new Resistor;
			nomeR = lineParameters[0];
			(r->addType)(nomeR[0]);
			nomeR.erase(nomeR.begin()); // remove the first letter, the component identifier
			(r->setName)(nomeR);
			(r->addNode)(stoul(lineParameters[1]));
			(r->addNode)(stoul(lineParameters[2]));
			(r->setValue)(stod(lineParameters[3]));
			componentes.push_back(r);
		}
		break;

		case 'I': // fonte de corrente independente
		{
			string nomeI;
			CurrentSource *i = new CurrentSource;
			nomeI = lineParameters[0];
			(i->addType)(nomeI[0]);
			nomeI.erase(nomeI.begin()); // remove the first letter, the component identifier
			(i->setName)(nomeI);
			(i->addNode)(stoul(lineParameters[1]));
			(i->addNode)(stoul(lineParameters[2]));
			(i->setValue)(stod(lineParameters[3]));
			(i->setPhase)(stod(lineParameters[4]));
			(i->setDCValue)(stod(lineParameters[5]));
			componentes.push_back(i);
		}
		break;

		case 'V': // fonte de corrente independente
		{
			string nomeV;
			VoltageSource *v = new VoltageSource;
			nomeV = lineParameters[0];
			(v->addType)(nomeV[0]);
			nomeV.erase(nomeV.begin()); // remove the first letter, the component identifier
			(v->setName)(nomeV);
			(v->addNode)(stoul(lineParameters[1]));
			(v->addNode)(stoul(lineParameters[2]));
			(v->setValue)(stod(lineParameters[3]));
			(v->setPhase)(stod(lineParameters[4]));
			(v->setDCValue)(stod(lineParameters[5]));
			extraRows += 1;
			(v->SetExtraPosition)(extraRows);
			componentes.push_back(v);
		}
		break;

        case 'E':
		{
			string nomeE;
			VoltageSrcCntrlVoltage *e = new VoltageSrcCntrlVoltage;
			nomeE = lineParameters[0];
			(e->addType)(nomeE[0]);
			nomeE.erase(nomeE.begin()); // remove the first letter, the component identifier
			(e->setName)(nomeE);
			(e->addNode)(stoul(lineParameters[1]));
			(e->addNode)(stoul(lineParameters[2]));
			(e->addNode)(stoul(lineParameters[3]));
			(e->addNode)(stoul(lineParameters[4]));
			(e->setValue)(stod(lineParameters[5]));
			extraRows += 1;
			(e->SetExtraPosition)(extraRows);
			componentes.push_back(e);
		}
		break;

        case 'F':
		{
			string nomeF;
			CurrentSrcCntrlCurrent *f = new CurrentSrcCntrlCurrent;
			nomeF = lineParameters[0];
			(f->addType)(nomeF[0]);
			nomeF.erase(nomeF.begin()); // remove the first letter, the component identifier
			(f->setName)(nomeF);
			(f->addNode)(stoul(lineParameters[1]));
			(f->addNode)(stoul(lineParameters[2]));
			(f->addNode)(stoul(lineParameters[3]));
			(f->addNode)(stoul(lineParameters[4]));
			(f->setValue)(stod(lineParameters[5]));
			extraRows += 1;
			(f->SetExtraPosition)(extraRows);
			componentes.push_back(f);
		}
		break;
        
		case 'G':
		{
			string nomeG;
			CurrentSrcCntrlVoltage *g = new CurrentSrcCntrlVoltage;
			nomeG = lineParameters[0];
			(g->addType)(nomeG[0]);
			nomeG.erase(nomeG.begin()); // remove the first letter, the component identifier
			(g->setName)(nomeG);
			(g->addNode)(stoul(lineParameters[1]));
			(g->addNode)(stoul(lineParameters[2]));
			(g->addNode)(stoul(lineParameters[3]));
			(g->addNode)(stoul(lineParameters[4]));
			(g->setValue)(stod(lineParameters[5]));
			componentes.push_back(g);
		}
		break;

        case 'H':
		{
			string nomeH;
			VoltageSrcCntrlCurrent *h = new VoltageSrcCntrlCurrent;
			nomeH = lineParameters[0];
			(h->addType)(nomeH[0]);
			nomeH.erase(nomeH.begin()); // remove the first letter, the component identifier
			(h->setName)(nomeH);
			(h->addNode)(stoul(lineParameters[1]));
			(h->addNode)(stoul(lineParameters[2]));
			(h->addNode)(stoul(lineParameters[3]));
			(h->addNode)(stoul(lineParameters[4]));
			(h->setValue)(stod(lineParameters[5]));
			extraRows += 1;
			(h->SetExtraPosition)(extraRows);
			extraRows += 1;
			(h->SetExtraPosition)(extraRows);
			componentes.push_back(h);
		}
		break;

		case 'C':
		{
			string nomeC;
			Capacitor *c = new Capacitor;
			nomeC = lineParameters[0];
			(c->addType)(nomeC[0]);
			nomeC.erase(nomeC.begin()); // remove the first letter, the component identifier
			(c->setName)(nomeC);
			(c->addNode)(stoul(lineParameters[1]));
			(c->addNode)(stoul(lineParameters[2]));
			(c->setValue)(stod(lineParameters[3]));
			(c->setInitialValue)(stod(lineParameters[4]));
			componentes.push_back(c);
		}
		break;

  		case 'L':
		{
			string nomeL;
			Indutor *l = new Indutor;
			nomeL = lineParameters[0];
			(l->addType)(nomeL[0]);
			nomeL.erase(nomeL.begin()); // remove the first letter, the component identifier
			(l->setName)(nomeL);
			(l->addNode)(stoul(lineParameters[1]));
			(l->addNode)(stoul(lineParameters[2]));
			(l->setValue)(stod(lineParameters[3]));
			(l->setInitialValue)(stod(lineParameters[4]));
			componentes.push_back(l);
		}
		break;

		case 'K':
		{
			string firstIndutor, secondIndutor, nomeK;
			Transformador *t = new Transformador;
			unsigned achouAmbos = 0;
			unsigned count, countAuxiliar = 0;
			vector<Componente*> auxiliar;
			unsigned firstIndutorPosition, secondIndutorPosition;

			firstIndutor = lineParameters[1];
			secondIndutor = lineParameters[2];

            nomeK = lineParameters[0];
			(t->addType)(nomeK[0]);
			nomeK.erase(nomeK.begin()); // remove the first letter, the component identifier
			(t->setName)(nomeK);

			while (!achouAmbos && count != sizeof(componentes))
			{
				unsigned achou1, achou2 = 0;
				if (componentes[count]->getName() == firstIndutor)
				{
					achou1 = 1;
					(t->addNode)(componentes[count]->getNode(1));
					(t->addNode)(componentes[count]->getNode(2));
					(t->setValueFirstIndutor)(componentes[count]->getValue());
					(t->setValueM)(stod(lineParameters[3]));
					firstIndutorPosition = count;
				}

				if ((componentes[count]->getName()) == secondIndutor)
				{
					achou2 = 1;
					(t->addNode)(componentes[count]->getNode(1));
					(t->addNode)(componentes[count]->getNode(2));
					(t->setValueSecondIndutor)(componentes[count]->getValue());
					secondIndutorPosition = count;
				}
				if (achou1 == 1 && achou2 == 1)
					achouAmbos = 1;

				count = count + 1;
			}

			count = 0;
			countAuxiliar = 0;
			while (count != (sizeof(componentes) - 1))
			{
				if (count != firstIndutorPosition && count != secondIndutorPosition)
				{
					auxiliar[countAuxiliar] = componentes[count];
					countAuxiliar = countAuxiliar + 1;
				}

				count = count + 1;
			}

			componentes = auxiliar;

			componentes.push_back(t);
		}
		break;

		case 'O':
		{
			string nomeO;
			AmpOp *o = new AmpOp;
			nomeO = lineParameters[0];
			(o->addType)(nomeO[0]);
			nomeO.erase(nomeO.begin()); // remove the first letter, the component identifier
			(o->setName)(nomeO);
			(o->addNode)(stoul(lineParameters[1]));
			(o->addNode)(stoul(lineParameters[2]));
			(o->addNode)(stoul(lineParameters[3]));
			(o->addNode)(stoul(lineParameters[4]));
			(o->SetExtraPosition)(extraRows);
			extraRows += 1;
			componentes.push_back(o);
		}
		break;

		}
     	index++;
	}

	netlistFile.close();
}


void Netlist::DoConductanceMatrix()
{
	setRowsValue(componentes);
	InitializeG_Matrix();
	unsigned count = 0;
	double value;

	while (count != sizeof(componentes) - 1)
	{
		if (componentes[count]->getType() == 'R')
		{
			value = 1/(componentes[count]->getValue());
			G_Matrix[componentes[count]->getNode(0)][componentes[count]->getNode(0)] += value;
			G_Matrix[componentes[count]->getNode(1)][componentes[count]->getNode(1)] += value;
			G_Matrix[componentes[count]->getNode(0)][componentes[count]->getNode(1)] -= value;
			G_Matrix[componentes[count]->getNode(1)][componentes[count]->getNode(0)] -= value;
		}

		else if (componentes[count]->getType() == 'G')
		{
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][componentes[count]->getNode(2)] += value;
			G_Matrix[componentes[count]->getNode(1)][componentes[count]->getNode(3)] += value;
			G_Matrix[componentes[count]->getNode(0)][componentes[count]->getNode(3)] -= value;
			G_Matrix[componentes[count]->getNode(1)][componentes[count]->getNode(2)] -= value;
		
		}

	    else if (componentes[count]->getType() == 'I')
		{
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][GetRows()+ extraRows + 1] -= value;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + extraRows + 1] += value;
		}

		else if (componentes[count]->getType() == 'V')
		{
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][GetRows() + componentes[count]->GetExtraPosition(1)] += 1;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + componentes[count]->GetExtraPosition(1)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)][componentes[count]->getNode(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)][componentes[count]->getNode(1)] += 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)][GetRows() + extraRows + 1] -= value;
		}

		else if (componentes[count]->getType() == 'E') 
		{
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][GetRows() + componentes[count]->GetExtraPosition(0)] += 1;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + componentes[count]->GetExtraPosition(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(1)] += 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(2)] += value;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(3)] -= value;
		}


		else if (componentes[count]->getType()  == 'F') 
		{
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][GetRows() + componentes[count]->GetExtraPosition(0)] += value;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + componentes[count]->GetExtraPosition(0)] -= value;
			G_Matrix[componentes[count]->getNode(2)][GetRows() + componentes[count]->GetExtraPosition(0)] += 1;
			G_Matrix[componentes[count]->getNode(3)][GetRows() + componentes[count]->GetExtraPosition(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(2)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(3)] += 1;
		}

		else if (componentes[count]->getType() == 'H') {
			value = componentes[count]->getValue();
			G_Matrix[componentes[count]->getNode(0)][GetRows() + componentes[count]->GetExtraPosition(1)] += 1;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + componentes[count]->GetExtraPosition(1)] -= 1;
			G_Matrix[componentes[count]->getNode(2)][GetRows() + componentes[count]->GetExtraPosition(0)] += 1;
			G_Matrix[componentes[count]->getNode(3)][GetRows() + componentes[count]->GetExtraPosition(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)][componentes[count]->getNode(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)][componentes[count]->getNode(1)] += 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(2)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(3)] += 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(1)]
				[GetRows() + componentes[count]->GetExtraPosition(0)] += value;
		}

		else if (componentes[count]->getType() == 'O')
		{
			G_Matrix[componentes[count]->getNode(0)][GetRows() + componentes[count]->GetExtraPosition(0)] += 1;
			G_Matrix[componentes[count]->getNode(1)][GetRows() + componentes[count]->GetExtraPosition(0)] -= 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(2)] += 1;
			G_Matrix[GetRows() + componentes[count]->GetExtraPosition(0)][componentes[count]->getNode(3)] -= 1;
		}


    
		count += 1;
	}
	

}



