#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <complex>
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

	void Mosfet::setPolarization(double Vdrain, double Vgate, double Vsource, double Vbulk)
	{
		unsigned auxiliarNode;
		double auxiliarV;

		VD = Vdrain;
		VG = Vgate;
		VS = Vsource;
		VB = Vbulk;

		if (mosType == 'N')
		{
			if (VS > VD)
			{
				auxiliarNode = nodes[0];
				nodes[0] = nodes[2];	
				nodes[2] = auxiliarNode;
				auxiliarV = VD;
				VD = VS;
				VS = auxiliarV;
				inverteu = !inverteu;
			}
		}
		else
		{
			if (VS < VD)
			{
				auxiliarNode = nodes[0];
				nodes[0] = nodes[2];
				nodes[2] = auxiliarNode;
				auxiliarV = VD;
				VD = VS;
				VS = auxiliarV;
				inverteu = !inverteu;
			}
		}

		VDS = VD - VS;
		VGS = VG - VS;
		VBS = VB - VS;

		if (mosType == 'N')
		{
			if (VBS > phi / 2)
				VBSaux = phi / 2;
			else
				VBSaux = VBS;
		}
		else
		{
			if ((-VBS) > phi / 2)
				VBSaux = (-phi) / 2;
			else
				VBSaux = VBS;
		}


		VT = Vt0 + gamma*(sqrt(phi - VBSaux) - sqrt(phi));


	}

	void Mosfet::setLinearParameters()
	{
		if (mosType == 'N')
		{
			if (VBS > phi / 2)
				VBSaux = phi / 2;
			else
				VBSaux = VBS;

			if (VGS < VT) //corte
			{
				Gds = Gm = Gmb = ID = I0 = 0;
				CGS = CGD = ((2 * k / 0.05)*largura*Ld);
				CBG = ((2 * k / 0.05)*comprimento*largura);
			}
			else if (VDS <= (VGS - VT)) //triodo
			{
				Gds = ((k*largura / comprimento)*(2 * (VGS - VT) - 2 * VDS + 4 *lambda*VDS*(VGS - VT) - 3 *lambda*(VDS*VDS)));
				Gm = ((k*largura / comprimento)*(2 * VDS*(1 + lambda*VDS)));
				Gmb = ((Gm*gamma) / (2 * sqrt(phi - VBSaux)));
				ID = ((k*largura / comprimento)*(2 * (VGS - VT)*VDS - (VDS*VDS))*(1 + lambda*VDS));
				I0 = ID - Gm*VGS - Gmb*VBSaux - Gds*VDS;	
				CGS = CGD = (((2 * k / 0.05)*largura*Ld) + ((k / 0.05)*comprimento*largura));
				CBG = 0;
			}
			else //ativo
			{
				Gds = ((k*largura / comprimento)*((VGS - VT)*(VGS - VT)*lambda));
				Gm = ((k*largura / comprimento)*(2 * (VGS - VT)*(1 + lambda*VDS)));
				Gmb = ((Gm*gamma) / (2 * sqrt(phi - VBSaux)));
				ID = ((k*largura / comprimento)*((VGS - VT)*(VGS - VT))*(1 + lambda*VDS));
				I0 = ID - Gm*VGS - Gmb*VBSaux - Gds*VDS;
				CGS = (((2*k / 0.05)*largura*Ld) + ((4*k / 0.15)*comprimento*largura));
				CGD = ((2*k / 0.05)*largura*Ld);
				CBG = 0;
			}

		}
		else //tipo P
		{
			if ( (-VBS) > phi / 2)
				VBSaux = (-phi) / 2;
			else
				VBSaux = VBS;

			if (VGS>(-VT)) //corte 
			{
				Gds = Gm = Gmb = I0 = ID = 0;
				CGS = CGD = ((2*k / 0.02)*largura*Ld);
				CBG = ((2*k / 0.02)*comprimento*largura);
			}
			else if ((-VDS) <= (-VGS - VT)) //triodo
			{
				Gds = ((k*largura / comprimento)*(2 * (-VGS - VT) - 2 * (-VDS) + 4 * lambda*(-VDS)*(-VGS - VT) - 3 * lambda*(VDS*VDS)));
				Gm = ((k*largura / comprimento)*(2 * (-VDS)*(1 + lambda*(-VDS))));
				Gmb = ((Gm*gamma) / (2 * sqrt(phi - VBSaux)));
				ID = ((k*largura / comprimento)*(2 * (-VGS - VT)*(-VDS) - (VDS*VDS))*(1 + lambda*(-VDS)));
				I0 = ID + Gm*VGS + Gmb*VBSaux + Gds*VDS;	
				CGS = CGD = (((2*k / 0.02)*largura*Ld) + ((k / 0.02)*comprimento*largura));
				CBG = 0;
			}
			else //ativo
			{
				Gds = ((k*largura / comprimento)*((-VGS - VT)*(-VGS - VT)*lambda));
				Gm = ((k*largura / comprimento)*(2 * (-VGS - VT)*(1 + lambda*(-VDS))));
				Gmb = ((Gm*gamma) / (2 * sqrt(phi - VBSaux)));
				ID = ((k*largura / comprimento)*((-VGS - VT)*(-VGS - VT))*(1 + lambda*(-VDS)));
				I0 = ID + Gm*VGS + Gmb*VBSaux + Gds*VDS;
				CGS = (((2*k / 0.02)*largura*Ld) + ((4*k / 0.06)*comprimento*largura));
				CGD = ((2*k / 0.02)*largura*Ld);
				CBG = 0;
			}


		}

		//printConditions();
	}
	void Mosfet::printConditions()
	{
		cout << "M";
		cout << getName();
		cout << " ";
		cout << mosType	;
		cout << " ";
		cout << "Gm: " << Gm ;
		cout << " Gds: " << Gds ;
		cout << " Gmb: " << Gmb ;
		cout << " Cgs: " << CGS ;
		cout << " Cgd: " << CGD ;
		cout << " Cbg: " << CBG ;
		cout << " VGS: " << VGS ;
		cout << " VDS: " << VDS ;
		cout << " ID: " << ID ;
		cout << " VT: " << VT;
		cout << " VBS: " << VBS;
		cout << " VBSaux: " << VBSaux << endl;
	}