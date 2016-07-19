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
#include "LinearSystem.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
char nomeArquivo[200+1];
char printer[20000];
FILE *arquivo;
boolean printEstampas = false;
LinearSystem sistemaLinear;
#define printf xprintf
void xprintf(char* format,...) /* Escreve no memo1 */
{
  va_list paramlist;
  char txt[2000];
  va_start(paramlist,format);
  vsprintf(txt,format,paramlist);
  Form1->Memo1->Lines->Add(txt);
}

int solveLinearSystem(char *filename){
   	Netlist netlist(filename);
	printf("FEZ NETLIST");
   //     printf(netlist.getTextoParaImprimirNaGui().c_str());
  //      strcpy(printer, netlist.getTextoParaImprimirNaGui().c_str());
 //       printf(printer);
 //	netlist.DoConductanceMatrixDC();
	printf("FEZ A MATRIZ");
 //	netlist.SistemaLinear.PrintG_Matrix();
 //	netlist.SistemaLinear.SolveLinearSystem();
	printf("RESOLVEU O SISTEMA");
 //	netlist.SistemaLinear.PrintG_Matrix();
 //	netlist.SistemaLinear.PrintVariables();
//	system("pause");

	return 0;
}

 void impressoraHP(char * textao){
     char linha[1000 + 1];
     int index = 0;
     int positionStartCopy = 0;
     memset(&linha[0], 0, sizeof(linha));
     for (index = 0; index < sizeof(textao); index++){
        if (textao[index] == '\n' || textao[index] == '\0'){
                strncpy(linha, &textao[positionStartCopy], index + 1);
                linha[index] = '\0';
                positionStartCopy = index + 1;
                printf(linha);
                printf(" ");
        }
     }
}



/////////////
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner){
}

void __fastcall TForm1::Abrir1Click(TObject *Sender){
denovo:
if (!OpenDialog1->Execute()) return;
  strcpy(nomeArquivo,OpenDialog1->FileName.c_str());
  arquivo=fopen(nomeArquivo,"r");
  if (arquivo==0) {
    printf("Arquivo %s inexistente",nomeArquivo);
    goto denovo;
    }
    printf("aeHO");
    string linha;
	ifstream netlistFile;
	unsigned index = 0;

	sistemaLinear.extraRows = 0;
        //c_str() converts to const char *
	netlistFile.open(nomeArquivo);
	if (!netlistFile)
	{
		printf("Nao foi possivel abrir o arquivo");
	}
        printf("Abrindo\n o arquivo no destino solicitado:");
    	printf(nomeArquivo);
        solveLinearSystem(nomeArquivo);
    return;
}
	

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  //printf("teste");
  return;
}

void __fastcall TForm1::ImprimirEstampas2Click(TObject *Sender)
{
        printEstampas = !printEstampas;
        if (printEstampas == true)
         printf("verdadeiro!");
}
//---------------------------------------------------------------------------

