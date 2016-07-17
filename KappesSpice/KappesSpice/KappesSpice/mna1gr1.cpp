#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <vcl.h>
#pragma hdrstop
#include "mna1gr1.h"
#include "Componente.h"
#include "Netlist.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

void xprintf(char* format,...) /* Escreve no memo1 */
{
  va_list paramlist;
  char txt[2000];
  va_start(paramlist,format);
  vsprintf(txt,format,paramlist);
  Form1->Memo1->Lines->Add(txt);
}

int solveLinearSystem(){
	//const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net";
	const char* netlistPath = "..\\..\\..\\simples.net";
	Netlist netlist(netlistPath);
	cout << "FEZ NETLIST" << endl;
	//Form1->Memo1->Lines->Add("FEZ NETLIST");
	netlist.DoConductanceMatrixDC();
	//Form1->Memo1->Lines->Add("FEZ A MATRIZ");
	cout << "FEZ A MATRIZ" << endl;
	netlist.SistemaLinear.PrintG_Matrix();
	netlist.SistemaLinear.SolveLinearSystem();
	//Form1->Memo1->Lines->Add("RESOLVEU O SISTEMA");
	cout << "RESOLVEU O SISTEMA" << endl;
	netlist.SistemaLinear.PrintG_Matrix();
	netlist.SistemaLinear.PrintVariables();
	system("pause");
	
	return 0;
}
/////////////
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner){
}

void __fastcall TForm1::Abrir1Click(TObject *Sender){
	solveLinearSystem();
}
	

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  printf("teste");
  return;
}




