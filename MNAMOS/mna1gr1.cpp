//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mna1gr1.h"
#include "menuavancado.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
Programa de demonstracao de analise nodal modificada
Por Antonio Carlos M. de Queiroz acmq@coe.ufrj.br
Versao 1.0 - 6/9/2000
Versao 1.0a - 8/9/2000 Ignora comentarios
Versao 1.0b - 15/9/2000 Aumentado Yn, retirado Js
Versao 1.0c - 19/2/2001 Mais comentarios
Versao 1.0d - 16/2/2003 Tratamento correto de nomes em minusculas
Versao 1.0e - 21/8/2008 Estampas sempre somam. Ignora a primeira linha
Versao 1.0f - 21/6/2009 Corrigidos limites de alocacao de Yn
Versao 1.0g - 15/10/2009 Le as linhas inteiras
Versao 1.0h - 18/6/2011 Estampas correspondendo a modelos
Versao 1.0i - 03/11/2013 Correcoes em *p e saida com sistema singular.
Versao 1.0j - 26/11/2015 Evita operacoes com zero.
*/

/*
Elementos aceitos e linhas do netlist:

Resistor:  R<nome> <no+> <no-> <resistencia>
Capacitor: C<nome> <no+> <no-> <capacitancia>
Indutor:   L<nome> <no+> <no-> <indutancia>
Acoplamento entre indutores: K<nome> <La> <Lb> <k> (La e Lb nomes de indutores j� declarados.)
VCCS:      G<nome> <io+> <io-> <vi+> <vi-> <transcondutancia>
VCVC:      E<nome> <vo+> <vo-> <vi+> <vi-> <ganho de tensao>
CCCS:      F<nome> <io+> <io-> <ii+> <ii-> <ganho de corrente>
CCVS:      H<nome> <vo+> <vo-> <ii+> <ii-> <transresistencia>
Fonte I:   I<nome> <io+> <io-> <modulo> <fase> <dc>
Fonte V:   V<nome> <vo+> <vo-> <modulo> <fase> <dc>
Amp. op.:  O<nome> <vo1> <vo2> <vi1> <vi2>
Transistor MOS:M<nome> <n�d> <n�g> <n�s> <n�b> <NMOS ou PMOS> L=<comprimento> W=<largura> <K> <Vt0> <lambda> <gamma> <fi> <Ld>

As fontes F e H tem o ramo de entrada em curto
O amplificador operacional ideal tem a saida suspensa
Os nos podem ser nomes
*/

#define versao "2.0 - 12/06/2016"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <complex.h>    /* Standard Library of Complex Numbers */
#include <time.h>       /* Seed for rand */
#define MAX_LINHA 80
#define MAX_NOMEARQUIVO 255
#define MAX_NOME 11
#define MAX_ELEM 50
#define MAX_NOS 50
#define MAX_ITERACOES 150
#define MAX_RANDOM 20
#define TOLERANCIA 1e-9
#define TOLERANCIAMENOR 1e-3
#define RESISTENCIADC 1e-9
#define TOLGINICIAL 1e-12
#define PI2 (8*atan(1)) /* Forma f�cil de calcular 2PI com a biblioteca math */
#define PI  (4*atan(1)) /* Forma f�cil de calcular PI com a biblioteca math */

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],npmos[MAX_NOME];
  double valor,comprimento,largura,k,vt,lambda,gamma,fi,ld;
  int a,b,c,d,x,y;
} elemento;

elemento netlist[MAX_ELEM]; /* Netlist */

int
  ne, /* Elementos */
  nv, /* Variaveis */
  nn, /* Nos */
  npontos, /* Pontos na escala */
  i,j,k,iteracoes,tentativas;

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  nomeArquivo[MAX_NOMEARQUIVO+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  escala[MAX_NOME],indutorA[MAX_NOME],indutorB[MAX_NOME], /* NOVO: LIN, OCT OU DEC, salva nome dos indutores pro acoplamento*/
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1],buf[1000],tmp[1000],
  *p,
  temTransistor, /* Saber se e necessario fazer an�lise do ponto de operacao */
  mostraEstampa,mostraNP,mostraMOS,mostraEstampaAC, iniciaConduzindo,
  temAc=0;
FILE *arquivo;


#define I dcomplex(0.0,1.0)
typedef std::complex<double> dcomplex;

dcomplex
  gC,
  YnC[MAX_NOS+1][MAX_NOS+2];

double
  g,gmb,gm,gds,id,cgs,cgd,cbg,vbsAux,frequenciaInicial,frequenciaFinal,
  passoFrequencia,tolg,resistenciaDC,tolerancia,  /* Frequencias e parametros transistor */
  solucaoAnterior[MAX_NOS+1],erro[MAX_NOS+1],
  Yn[MAX_NOS+1][MAX_NOS+2];

#define printf xprintf
void xprintf(char* format,...) /* Escreve no memo1 */
{
  va_list paramlist;
  char txt[2000];
  va_start(paramlist,format);
  vsprintf(txt,format,paramlist);
  Form1->Memo1->Lines->Add(txt);
}

/* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal e variaveis reais*/
int ResolverSistemaR(void)
{
  int i,j,l, a;
  double t, p;

  for (i=1; i<=nv; i++) {
    t=0.0;
    a=i;
    for (l=i; l<=nv; l++) {
      if (fabs(Yn[l][i])>fabs(t)) {
	a=l;
	t=Yn[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=nv+1; l++) {
	p=Yn[i][l];
	Yn[i][l]=Yn[a][l];
	Yn[a][l]=p;
      }
    }
    if (fabs(t)<tolg) {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=nv+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j]/= t;
      p=Yn[i][j];
      if (p!=0)  /* Evita operacoes com zero */
        for (l=1; l<=nv; l++) {
      if (l!=i)
      Yn[l][j]-=Yn[l][i]*p;
        }
    }
  }
  return 0;
}

 /* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal e variaveis complexas*/
int ResolverSistemaC(void)
{
  int i,j,l, a;
  dcomplex t, p;

  for (i=1; i<=nv; i++) {
    t=0.0;
    a=i;
    for (l=i; l<=nv; l++) {
      if (abs(YnC[l][i])>abs(t)) {
	a=l;
	t=YnC[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=nv+1; l++) {
	p=YnC[i][l];
	YnC[i][l]=YnC[a][l];
	YnC[a][l]=p;
      }
    }
    if (abs(t)<tolg) {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=nv+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      YnC[i][j]/= t;
      p=YnC[i][j];
      if (p!=(0.0,0.0))  /* Evita operacoes com zero */
        for (l=1; l<=nv; l++) {  
	  if (l!=i)
	    YnC[l][j]-=YnC[l][i]*p;
        }
    }
  }
  return 0;
}

/* Grava a solucao na frequencia atual no arquivo */
int GravarLinha(void)
{
    fprintf (arquivo,"%lg ", frequenciaInicial);
    for (i=1; i<nv; i++) {
	if (YnC[i][nv+1]!=(0.0,0.0)) fprintf(arquivo,"%lg %lg ",abs(YnC[i][nv+1]),arg(YnC[i][nv+1])*180/PI);
	else fprintf(arquivo,"%lg %lg ",0.0,0.0);
	}
	if (YnC[i][nv+1]!=(0.0,0.0)) fprintf(arquivo,"%lg %lg \n",abs(YnC[i][nv+1]),arg(YnC[i][nv+1])*180/PI);
	else fprintf(arquivo,"%lg %lg \n",0.0,0.0);
    return 0;
}

/* Rotina que conta os nos e atribui numeros a eles */
int Numero(char *nome,int *no)
{
  int i,achou;

  i=0; achou=0;
  while (!achou && i<=nv) if (!(achou=!strcmp(nome,lista[i]))) i++;
  if (!achou) {
    if (nv==MAX_NOS) {
      printf("O programa so aceita ate %d nos\n",nv);
      return 1;
    }
    nv++;
    strcpy(lista[nv],nome);
    *no= nv;
    return 0; /* novo no */
  }
  else {
    *no= i;
    return 0; /* no ja conhecido */
  }
}

/* Localiza os indutores pelo nome para o acoplamento */
int LocalizaIndutores(char *indutor, int *indice)
{
  int i,achou;

  i=1; achou=0;
  while (!achou && i<=ne) {
    if (!(achou=!strcmp(indutor,netlist[i].nome))) i++;
    }
    if (!achou) {
      printf("O indutor %s do acoplamento nao foi encontrado\n",indutor);
      return 1;
  }
  else {
    *indice=i;
    return 0;  /* Posi��o do indutor na netlist */
  }
}

int MontaEstampaDC(char convergiu)
{
  /* Zera sistema */
  for (i=0; i<=nv; i++) {
    for (j=0; j<=nv+1; j++){
      Yn[i][j]=0;
    }
  }
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R' && !convergiu) {
        g=1/netlist[i].valor;
        Yn[netlist[i].a][netlist[i].a]+=g;
        Yn[netlist[i].b][netlist[i].b]+=g;
        Yn[netlist[i].a][netlist[i].b]-=g;
        Yn[netlist[i].b][netlist[i].a]-=g;
    }
    else if (tipo=='G' && !convergiu) {
      g=netlist[i].valor;
      Yn[netlist[i].a][netlist[i].c]+=g;
      Yn[netlist[i].b][netlist[i].d]+=g;
      Yn[netlist[i].a][netlist[i].d]-=g;
      Yn[netlist[i].b][netlist[i].c]-=g;
    }
    else if (tipo=='I' && !convergiu) {
      g=netlist[i].vt;
      Yn[netlist[i].a][nv+1]-=g;
      Yn[netlist[i].b][nv+1]+=g;
    }
    else if (tipo=='V' && !convergiu) {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=netlist[i].vt;
    }
    else if (tipo=='E' && !convergiu) {
      g=netlist[i].valor;
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][netlist[i].c]+=g;
      Yn[netlist[i].x][netlist[i].d]-=g;
    }
    else if (tipo=='F' && !convergiu) {
      g=netlist[i].valor;
      Yn[netlist[i].a][netlist[i].x]+=g;
      Yn[netlist[i].b][netlist[i].x]-=g;
      Yn[netlist[i].c][netlist[i].x]+=1;
      Yn[netlist[i].d][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].c]-=1;
      Yn[netlist[i].x][netlist[i].d]+=1;
    }
    else if (tipo=='H' && !convergiu) {
      g=netlist[i].valor;
      Yn[netlist[i].a][netlist[i].y]+=1;
      Yn[netlist[i].b][netlist[i].y]-=1;
      Yn[netlist[i].c][netlist[i].x]+=1;
      Yn[netlist[i].d][netlist[i].x]-=1;
      Yn[netlist[i].y][netlist[i].a]-=1;
      Yn[netlist[i].y][netlist[i].b]+=1;
      Yn[netlist[i].x][netlist[i].c]-=1;
      Yn[netlist[i].x][netlist[i].d]+=1;
      Yn[netlist[i].y][netlist[i].x]+=g;
    }
    else if (tipo=='O' && !convergiu) {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].c]+=1;
      Yn[netlist[i].x][netlist[i].d]-=1;
    }
    else if (tipo=='C' && !convergiu) {
      g=RESISTENCIADC;
      Yn[netlist[i].a][netlist[i].a]+=g;
      Yn[netlist[i].b][netlist[i].b]+=g;
      Yn[netlist[i].a][netlist[i].b]-=g;
      Yn[netlist[i].b][netlist[i].a]-=g;
    }
    else if (tipo=='L' && !convergiu) {
      g=RESISTENCIADC;//(0.1*sqrt(netlist[i].valor));
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][netlist[i].x]+=g;
    }
    else if (tipo=='M') {
      #define VD  (solucaoAnterior[netlist[i].a])
      #define VG  (solucaoAnterior[netlist[i].b])
      #define VB  (solucaoAnterior[netlist[i].d])
      #define VS  (solucaoAnterior[netlist[i].c])
      #define VGS (solucaoAnterior[netlist[i].b]-solucaoAnterior[netlist[i].c])
      #define VDS (solucaoAnterior[netlist[i].a]-solucaoAnterior[netlist[i].c])
      #define VBS (solucaoAnterior[netlist[i].d]-solucaoAnterior[netlist[i].c])
      #define VT  (netlist[i].vt + netlist[i].gamma*(sqrt(netlist[i].fi - vbsAux) - sqrt(netlist[i].fi)))

      if (netlist[i].npmos[0]=='N'){
        if (VD<VS) { /* Se Vd < Vs ent�o troca drain e source */
          netlist[i].x=!netlist[i].x; /*usa a variavel x pra acompanhar se o transistor inverteu */
          j=netlist[i].a;
          netlist[i].a=netlist[i].c;
          netlist[i].c=j;
        }
        vbsAux = (VBS>netlist[i].fi/2)? (netlist[i].fi/2):VBS;
        if (iniciaConduzindo && !tentativas && !iteracoes){
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((2*VT-VT)*(2*VT-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(2*VT-VT)*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((2*VT-VT)*(2*VT-VT))*(1+netlist[i].lambda*VDS)) -gm*2*VT -gmb*vbsAux - gds*VDS;
        }
        else if (VGS<VT) {
         netlist[i].y=0; /* aproveita a variavel y para guardar o ponto de operacao*/
         gds=gm=gmb=g=id=0;
         cgs=cgd=((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld);
         cbg=((2*netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura);
        }
        else if (VDS <= (VGS-VT)) {
            netlist[i].y=1;
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)-2*VDS+4*netlist[i].lambda*VDS*(VGS-VT)-3*netlist[i].lambda*(VDS*VDS)));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*VDS*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)*VDS-(VDS*VDS))*(1+netlist[i].lambda*VDS)) -gm*VGS -gmb*vbsAux - gds*VDS;
            id=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)*VDS-(VDS*VDS))*(1+netlist[i].lambda*VDS));
            cgs=cgd=(((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld)+((netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura));
            cbg=0;
        }
        else {
            netlist[i].y=2;
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((VGS-VT)*(VGS-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((VGS-VT)*(VGS-VT))*(1+netlist[i].lambda*VDS)) -gm*VGS -gmb*vbsAux - gds*VDS;
            id=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((VGS-VT)*(VGS-VT))*(1+netlist[i].lambda*VDS));
            cgs=(((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld)+((4*netlist[i].k/0.15)*netlist[i].comprimento*netlist[i].largura));
            cgd=((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld);
            cbg=0;
        }
          /*transcondutancia 1  */

          Yn[netlist[i].a][netlist[i].d]+=gmb;
          Yn[netlist[i].c][netlist[i].c]+=gmb;
          Yn[netlist[i].a][netlist[i].c]-=gmb;
          Yn[netlist[i].c][netlist[i].d]-=gmb;
          
           /*transcondutancia 2  */

          Yn[netlist[i].a][netlist[i].b]+=gm;
          Yn[netlist[i].c][netlist[i].c]+=gm;
          Yn[netlist[i].a][netlist[i].c]-=gm;
          Yn[netlist[i].c][netlist[i].b]-=gm;
          /* I0 */
          Yn[netlist[i].a][nv+1]-=g;
          Yn[netlist[i].c][nv+1]+=g;
      }
      else { /* PMOS */ 
          if (VD>VS) { /* Se Vd > Vs ent�o troca drain e source */
          netlist[i].x=!netlist[i].x; /*usa a variavel x pra acompanhar se o transistor inverteu */
          j=netlist[i].a;
          netlist[i].a=netlist[i].c;
          netlist[i].c=j;
        }
      
        vbsAux = ((-VBS)>netlist[i].fi/2)? (-netlist[i].fi/2):VBS;
        if (iniciaConduzindo && !tentativas && !iteracoes){
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-2*VT-VT)*(2*VT-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-2*VT-VT)*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-2*VT-VT)*(-2*VT-VT))*(1+netlist[i].lambda*VDS)) -gm*-2*VT -gmb*vbsAux - gds*VDS;
        }
        if (VGS>(-VT)) {
         netlist[i].y=0; /* aproveita a variavel y para guardar o ponto de operacao*/
         gds=gm=gmb=g=id=0;
         cgs=cgd=((2*netlist[i].k/0.02)*netlist[i].largura*netlist[i].ld);
         cbg=((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].largura);
        }
        else if ((-VDS) <= (-VGS-VT)) {
            netlist[i].y=1;
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)-2*(-VDS)+4*netlist[i].lambda*(-VDS)*(-VGS-VT)-3*netlist[i].lambda*(VDS*VDS)));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VDS)*(1+netlist[i].lambda*(-VDS))));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)*(-VDS)-(VDS*VDS))*(1+netlist[i].lambda*(-VDS))) +gm*VGS +gmb*vbsAux +gds*VDS;
            id=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)*(-VDS)-(VDS*VDS))*(1+netlist[i].lambda*(-VDS)));
            cgs=cgd=(((2*netlist[i].k/0.02)*netlist[i].largura*netlist[i].ld)+((netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura));
            cbg=0;
        }
        else {
            netlist[i].y=2;
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-VGS-VT)*(-VGS-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)*(1+netlist[i].lambda*(-VDS))));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            g=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-VGS-VT)*(-VGS-VT))*(1+netlist[i].lambda*(-VDS))) +gm*VGS +gmb*vbsAux +gds*VDS;
            id=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-VGS-VT)*(-VGS-VT))*(1+netlist[i].lambda*(-VDS)));
            cgs=(((2*netlist[i].k/0.02)*netlist[i].largura*netlist[i].ld)+((4*netlist[i].k/0.06)*netlist[i].comprimento*netlist[i].largura));
            cgd=((2*netlist[i].k/0.02)*netlist[i].largura*netlist[i].ld);
            cbg=0;
        }
          /*transcondutancia 1  */
          
          Yn[netlist[i].c][netlist[i].c]+=gmb;
          Yn[netlist[i].a][netlist[i].d]+=gmb;
          Yn[netlist[i].c][netlist[i].d]-=gmb;
          Yn[netlist[i].a][netlist[i].c]-=gmb;

           /*transcondutancia 2  */

          Yn[netlist[i].c][netlist[i].c]+=gm;
          Yn[netlist[i].a][netlist[i].b]+=gm;
          Yn[netlist[i].c][netlist[i].b]-=gm;
          Yn[netlist[i].a][netlist[i].c]-=gm;

          /* I0 */
          Yn[netlist[i].c][nv+1]-=g;
          Yn[netlist[i].a][nv+1]+=g;
      }

      if (mostraMOS || convergiu) {
		  printf("\n%s: %c %s %s%s%s VGS:%g VDS:%g vbsCalculado:%g vbsUsado:%g gmb:%g gm:%g gds:%g I0:%g ID:%g cgs:%g cgd:%g cbg:%g VD:%g VG:%g VS:%g VB:%g VT:%g\n",
		  netlist[i].nome,netlist[i].npmos[0],netlist[i].x? "invertido":"normal",netlist[i].y==0? "corte":"",netlist[i].y==1? "triodo":"",netlist[i].y==2? "saturado":"",
                  VGS,VDS,VBS,vbsAux,gmb,gm,gds,g,id,cgs,cgd,cbg,VD,VG,VS,VB,VT);
      }

      /*transcondutancia gds = condutancia apenas */

      Yn[netlist[i].a][netlist[i].a]+=gds;
      Yn[netlist[i].c][netlist[i].c]+=gds;
      Yn[netlist[i].a][netlist[i].c]-=gds;
      Yn[netlist[i].c][netlist[i].a]-=gds;

      /*capacitancia 1 CGD */
      
      Yn[netlist[i].a][netlist[i].a]+=RESISTENCIADC;
      Yn[netlist[i].b][netlist[i].b]+=RESISTENCIADC;
      Yn[netlist[i].a][netlist[i].b]-=RESISTENCIADC;
      Yn[netlist[i].b][netlist[i].a]-=RESISTENCIADC;
      
      /*capacitancia 2 CGS */

      Yn[netlist[i].b][netlist[i].b]+=RESISTENCIADC;
      Yn[netlist[i].c][netlist[i].c]+=RESISTENCIADC;
      Yn[netlist[i].b][netlist[i].c]-=RESISTENCIADC;
      Yn[netlist[i].c][netlist[i].b]-=RESISTENCIADC;
      
      /*capacitancia 3 CBG */
      Yn[netlist[i].d][netlist[i].d]+=RESISTENCIADC;
      Yn[netlist[i].b][netlist[i].b]+=RESISTENCIADC;
      Yn[netlist[i].b][netlist[i].d]-=RESISTENCIADC;
      Yn[netlist[i].d][netlist[i].b]-=RESISTENCIADC;
    }
    if (mostraEstampa && !convergiu) { //(mostraEstampa && !tentativas && !iteracoes) {
      printf("Sistema apos a estampa de %s (iteracao=%d tentativa=%d)\n",netlist[i].nome,iteracoes,tentativas);
      for (k=1; k<=nv; k++) {
        strcpy(buf,"");
        for (j=1; j<=nv+1; j++) {
          if (Yn[k][j]!=0) sprintf(tmp,"%+6.2e ",Yn[k][j]);
          else sprintf(tmp,"......... ");
          strcat(buf,tmp);
        }
        printf(buf);
      }
    }
  }
  return 0;
}

/* Cria o arquivo com o ponto de operacao e o arquivo tab */
/* Se temAc==0 so cria o arquivo do ponto de operacao */
int InicializarArquivos(char temAc)
{
        nomeArquivo[strlen(nomeArquivo)-3]='\0';
        strcat(nomeArquivo,"dc");
        arquivo=fopen(nomeArquivo,"w");
        if (!arquivo)
        {
            printf("Erro criando arquivo %s com o ponto de operacao\n",nomeArquivo);
            return 1;
        }
        for (i=1; i<=nv; i++)  fprintf(arquivo,"%s ",lista[i]);
        fprintf(arquivo,"\n");
        for (i=1; i<=nv; i++) fprintf (arquivo,"%lg ",Yn[i][nv+1]);
        fclose(arquivo);
        printf("\nPonto de Operacao:\n");
        strcpy(txt,"Tensao");
        for (i=0; i<=nv; i++) {
          if (i==nn+1) strcpy(txt,"Corrente");
          printf("%s %s: %g\n",txt,lista[i],Yn[i][nv+1]);
        }
        MontaEstampaDC(1); /* Printar parametros MOS, passa convergiu =1*/
        printf("\nArquivo %s criado com o ponto de operacao\n",nomeArquivo);
        if (temAc){
        nomeArquivo[strlen(nomeArquivo)-2]='\0';
        strcat(nomeArquivo,"tab");
        arquivo=fopen(nomeArquivo,"w");
        if (!arquivo)
        {
            printf("Erro criando arquivo %s com a resposta em frequencia\n",nomeArquivo);
            return 1;
        }
        fprintf(arquivo, "f ");
        for (i=1; i<=nv; i++)  fprintf(arquivo,"%sm %sf ",lista[i],lista[i]);
        fprintf(arquivo,"\n");
        return 0;
        }
        else {
        printf("Linha .AC com especificacoes da frequencia nao foi detectada.\nSera feito apenas o ponto de operacao\n");
        return 1;
        }
}

int MontaEstampaAC(void)
{
    /* Zera sistema */
  for (i=0; i<=nv; i++) {
    for (j=0; j<=nv+1; j++){
      YnC[i][j]=0;
    }
  }
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R') {
      gC=1/netlist[i].valor;
      YnC[netlist[i].a][netlist[i].a]+=gC;
      YnC[netlist[i].b][netlist[i].b]+=gC;
      YnC[netlist[i].a][netlist[i].b]-=gC;
      YnC[netlist[i].b][netlist[i].a]-=gC;
    }
    else if (tipo=='G') {
      gC=netlist[i].valor;
      YnC[netlist[i].a][netlist[i].c]+=gC;
      YnC[netlist[i].b][netlist[i].d]+=gC;
      YnC[netlist[i].a][netlist[i].d]-=gC;
      YnC[netlist[i].b][netlist[i].c]-=gC;
    }
    else if (tipo=='I') {
      gC=(netlist[i].valor*cos((PI/180)*netlist[i].fi) + I*netlist[i].valor*sin((PI/180)*netlist[i].fi));
      YnC[netlist[i].a][nv+1]-=gC;
      YnC[netlist[i].b][nv+1]+=gC;
    }
    else if (tipo=='V') {
      YnC[netlist[i].a][netlist[i].x]+=1;
      YnC[netlist[i].b][netlist[i].x]-=1;
      YnC[netlist[i].x][netlist[i].a]-=1;
      YnC[netlist[i].x][netlist[i].b]+=1;
      YnC[netlist[i].x][nv+1]-=(netlist[i].valor*cos((PI/180)*netlist[i].fi) + I*netlist[i].valor*sin((PI/180)*netlist[i].fi));
    }
    else if (tipo=='E') {
      gC=netlist[i].valor;
      YnC[netlist[i].a][netlist[i].x]+=1;
      YnC[netlist[i].b][netlist[i].x]-=1;
      YnC[netlist[i].x][netlist[i].a]-=1;
      YnC[netlist[i].x][netlist[i].b]+=1;
      YnC[netlist[i].x][netlist[i].c]+=gC;
      YnC[netlist[i].x][netlist[i].d]-=gC;
    }
    else if (tipo=='F') {
      gC=netlist[i].valor;
      YnC[netlist[i].a][netlist[i].x]+=gC;
      YnC[netlist[i].b][netlist[i].x]-=gC;
      YnC[netlist[i].c][netlist[i].x]+=1;
      YnC[netlist[i].d][netlist[i].x]-=1;
      YnC[netlist[i].x][netlist[i].c]-=1;
      YnC[netlist[i].x][netlist[i].d]+=1;
    }
    else if (tipo=='H') {
      gC=netlist[i].valor;
      YnC[netlist[i].a][netlist[i].y]+=1;
      YnC[netlist[i].b][netlist[i].y]-=1;
      YnC[netlist[i].c][netlist[i].x]+=1;
      YnC[netlist[i].d][netlist[i].x]-=1;
      YnC[netlist[i].y][netlist[i].a]-=1;
      YnC[netlist[i].y][netlist[i].b]+=1;
      YnC[netlist[i].x][netlist[i].c]-=1;
      YnC[netlist[i].x][netlist[i].d]+=1;
      YnC[netlist[i].y][netlist[i].x]+=gC;
    }
    else if (tipo=='O') {
      YnC[netlist[i].a][netlist[i].x]+=1;
      YnC[netlist[i].b][netlist[i].x]-=1;
      YnC[netlist[i].x][netlist[i].c]+=1;
      YnC[netlist[i].x][netlist[i].d]-=1;
    }
    else if (tipo=='C') {
      gC=I*PI2*frequenciaInicial*netlist[i].valor;
      YnC[netlist[i].a][netlist[i].a]+=gC;
      YnC[netlist[i].b][netlist[i].b]+=gC;
      YnC[netlist[i].a][netlist[i].b]-=gC;
      YnC[netlist[i].b][netlist[i].a]-=gC;
    }
    else if (tipo=='L') {
      gC=I*PI2*frequenciaInicial*netlist[i].valor;
      YnC[netlist[i].a][netlist[i].x]+=1;
      YnC[netlist[i].b][netlist[i].x]-=1;
      YnC[netlist[i].x][netlist[i].a]-=1;
      YnC[netlist[i].x][netlist[i].b]+=1;
      YnC[netlist[i].x][netlist[i].x]+=gC;
    }
    else if (tipo=='K') {
      netlist[i].x=netlist[netlist[i].a].x;
      netlist[i].y=netlist[netlist[i].b].x;
      gC=I*PI2*frequenciaInicial*netlist[i].valor*sqrt(netlist[netlist[i].a].valor*netlist[netlist[i].b].valor);
      YnC[netlist[i].x][netlist[i].y]+=gC;
      YnC[netlist[i].y][netlist[i].x]+=gC;
    }
    else if (tipo=='M') {
      vbsAux = (VBS>(netlist[i].fi/2))? (netlist[i].fi/2):VBS;
      if (netlist[i].npmos[0]=='N'){
        if (VD<VS) { /* Se Vd < Vs ent�o troca drain e source */
          j=netlist[i].a;
          netlist[i].a=netlist[i].c;
          netlist[i].c=j;
        }
        vbsAux = (VBS>netlist[i].fi/2)? (netlist[i].fi/2):VBS;
        
        if (VGS<VT) {
         gds=gm=gmb=0;
         cgs=cgd=((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld);
         cbg=((2*netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura);
        }
        else if (VDS <= (VGS-VT)) {
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)-2*VDS+4*netlist[i].lambda*VDS*(VGS-VT)-3*netlist[i].lambda*(VDS*VDS)));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*VDS*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            cgs=cgd=(((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld)+((netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura));
            cbg=0;
        }
        else {
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((VGS-VT)*(VGS-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(VGS-VT)*(1+netlist[i].lambda*VDS)));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            cgs=(((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld)+((4*netlist[i].k/0.15)*netlist[i].comprimento*netlist[i].largura));
            cgd=((2*netlist[i].k/0.05)*netlist[i].largura*netlist[i].ld);
            cbg=0;
        }
          /*transcondutancia 1  */
          
          YnC[netlist[i].a][netlist[i].d]+=gmb;
          YnC[netlist[i].c][netlist[i].c]+=gmb;
          YnC[netlist[i].a][netlist[i].c]-=gmb;
          YnC[netlist[i].c][netlist[i].d]-=gmb;
          
           /*transcondutancia 2  */

          YnC[netlist[i].a][netlist[i].b]+=gm;
          YnC[netlist[i].c][netlist[i].c]+=gm;
          YnC[netlist[i].a][netlist[i].c]-=gm;
          YnC[netlist[i].c][netlist[i].b]-=gm;
      }
      else { /* PMOS */ 
          if (VD>VS) { /* Se Vd > Vs ent�o troca drain e source */
          j=netlist[i].a;
          netlist[i].a=netlist[i].c;
          netlist[i].c=j;
        }
      
        vbsAux = ((-VBS)>netlist[i].fi/2)? (-netlist[i].fi/2):VBS;

        if (VGS>(-VT)) {
         gds=gm=gmb=0;
         cgs=cgd=((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].ld);
         cbg=((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].largura);
        }
        else if ((-VDS) <= (-VGS-VT)) {
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)-2*(-VDS)+4*netlist[i].lambda*(-VDS)*(-VGS-VT)-3*netlist[i].lambda*(VDS*VDS)));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VDS)*(1+netlist[i].lambda*(-VDS))));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            cgs=cgd=(((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].ld)+((netlist[i].k/0.05)*netlist[i].comprimento*netlist[i].largura));
            cbg=0;
        }
        else {
            gds=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-VGS-VT)*(-VGS-VT)*netlist[i].lambda));
            gm=((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*(2*(-VGS-VT)*(1+netlist[i].lambda*(-VDS))));
            gmb=((gm*netlist[i].gamma)/(2*sqrt(netlist[i].fi - vbsAux)));
            cgs=(((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].ld)+((4*netlist[i].k/0.06)*netlist[i].comprimento*netlist[i].largura));
            cgd=((2*netlist[i].k/0.02)*netlist[i].comprimento*netlist[i].ld);
            cbg=0;
        }
          /*transcondutancia 1  */
          
          YnC[netlist[i].c][netlist[i].c]+=gmb;
          YnC[netlist[i].a][netlist[i].d]+=gmb;
          YnC[netlist[i].c][netlist[i].d]-=gmb;
          YnC[netlist[i].a][netlist[i].c]-=gmb;
          
           /*transcondutancia 2  */

          YnC[netlist[i].c][netlist[i].c]+=gm;
          YnC[netlist[i].a][netlist[i].b]+=gm;
          YnC[netlist[i].c][netlist[i].b]-=gm;
          YnC[netlist[i].a][netlist[i].c]-=gm;
      }
      
      #ifdef DEBUGMOS
      printf("\n%s VD:%g VS:%g VGS:%g VDS:%g VBS:%g VT:%g vbsAux:%g gmb:%g gm:%g gds:%g I0:%g ID:%g cgs:%g cgd:%g cbg:%g\n",netlist[i].nome,VD,VS,VGS,VDS,VBS,VT,vbsAux,gmb,gm,gds,g,
      ((netlist[i].k*netlist[i].largura/netlist[i].comprimento)*((-VGS-VT)*(-VGS-VT))*(1+netlist[i].lambda*(-VDS))),cgs,cgd,cbg);
      //getch();
      #endif

      /*transcondutancia gds = condutancia apenas */
      
      YnC[netlist[i].a][netlist[i].a]+=gds;
      YnC[netlist[i].c][netlist[i].c]+=gds;
      YnC[netlist[i].a][netlist[i].c]-=gds;
      YnC[netlist[i].c][netlist[i].a]-=gds;

      /*capacitancia 1 CGD */
      
      YnC[netlist[i].a][netlist[i].a]+=I*PI2*frequenciaInicial*cgd;
      YnC[netlist[i].b][netlist[i].b]+=I*PI2*frequenciaInicial*cgd;
      YnC[netlist[i].a][netlist[i].b]-=I*PI2*frequenciaInicial*cgd;
      YnC[netlist[i].b][netlist[i].a]-=I*PI2*frequenciaInicial*cgd;
      
      /*capacitancia 2 CGS */

      YnC[netlist[i].b][netlist[i].b]+=I*PI2*frequenciaInicial*cgs;
      YnC[netlist[i].c][netlist[i].c]+=I*PI2*frequenciaInicial*cgs;
      YnC[netlist[i].b][netlist[i].c]-=I*PI2*frequenciaInicial*cgs;
      YnC[netlist[i].c][netlist[i].b]-=I*PI2*frequenciaInicial*cgs;
      
      /*capacitancia 3 CBG */
      YnC[netlist[i].d][netlist[i].d]+=I*PI2*frequenciaInicial*cbg;
      YnC[netlist[i].b][netlist[i].b]+=I*PI2*frequenciaInicial*cbg;
      YnC[netlist[i].b][netlist[i].d]-=I*PI2*frequenciaInicial*cbg;
      YnC[netlist[i].d][netlist[i].b]-=I*PI2*frequenciaInicial*cbg;
    }
    if (mostraEstampaAC) {
      printf("Sistema ap�s a estampa de %s (f=%lg)\n",netlist[i].nome,frequenciaInicial);
      for (k=1; k<=nv; k++) {
        strcpy(buf,"");
        for (j=1; j<=nv+1; j++) {
          if (abs(YnC[k][j])!=0) sprintf(tmp,"%+6.2e%+6.2ei ",real(YnC[k][j]),imag(YnC[k][j]));
          else sprintf(tmp,"......... ......... ");
          strcat(buf,tmp);
        }
        printf(buf);
      }
    }
  }
  return 0;
}

/* Resolucao de sistema de equacoes n�o lineares.
   Metodo de Newton-Raphson para convergir a partir 
   da solu��o Gauss-Jordan com condensacao pivotal e variaveis reais*/
int ResolverNewtonRaphson(void)
{
  double maiorErro=-1; /* erro -1 sera usado para sistema singular */
  srand(time(NULL));
  
  for (tentativas=0; tentativas<=MAX_RANDOM && fabs(maiorErro)>TOLERANCIA; tentativas++) {
    for (i=1; i<=nv; i++) {
      if (tentativas) {
        if (i<=nn) {
          if (erro[i]>TOLERANCIAMENOR || maiorErro==-1) { /* Randomiza se nao convergiu com tolerancia menor. erro -1 usado para sistema singular */
            solucaoAnterior[i]=(((rand() % 2000)/100.0)-10); /* Inicializa todas solucoes =0.1 e depois randomiza tensoes de -10 a 10 */
          }
        }
        else {
          if (erro[i]>TOLERANCIAMENOR || maiorErro==-1) {
            solucaoAnterior[i]=(((rand() % 200)/100.0)-1); /* Inicializa todas solucoes =0.1 e depois randomiza correntes de -1 a 1 */
          }
        }
      }
      else solucaoAnterior[i]=0.1;
    }
	if (mostraNP){
    printf("\n\nSistema inicial aproximado:\n");
    /* Mostra solucao */
    strcpy(txt,"Tensao");
    for (i=1; i<=nv; i++) {
      if (i==nn+1) strcpy(txt,"Corrente");
      printf("%s %s: %g\n",txt,lista[i],solucaoAnterior[i]);
    }
    getch();
    }	
    
    for (iteracoes=0; iteracoes<=MAX_ITERACOES && fabs(maiorErro)>TOLERANCIA; iteracoes++) { /* Enquanto erro maior que tolerancia e iteracoes abaixo do limite */
      for (i=1;i<=nv && iteracoes;i++) {
        if (i==1) erro[i]=maiorErro=(Yn[i][nv+1]>TOLERANCIA)? fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]):fabs(solucaoAnterior[i]-Yn[i][nv+1]); /* Primeiro erro conferindo relativo ou absoluto */
        else if (Yn[i][nv+1]>TOLERANCIA) {  /* Se o valor e grande usa erro relativo*/
          maiorErro= fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i])>maiorErro? fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]):maiorErro; /* Salva o maior erro */
          erro[i]=fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]);
        }
        else {  /* Se o valor e pequeno usa erro absoluto */
          maiorErro= fabs(solucaoAnterior[i]-Yn[i][nv+1])>maiorErro? fabs(solucaoAnterior[i]-Yn[i][nv+1]):maiorErro;
          erro[i]=fabs(solucaoAnterior[i]-Yn[i][nv+1]);
        }
      }
      
        if (mostraNP) {
        /* Opcional: Mostra o sistema resolvido */
        /* Mostra solucao */
        printf("\n\nSolucao Atual e Anterior e erro:\n");
        strcpy(txt,"Tensao");
        for (i=0; i<=nv; i++) {
          if (i==nn+1) strcpy(txt,"Corrente");
          printf("%s %s: %g %g %g\n",txt,lista[i],Yn[i][nv+1],solucaoAnterior[i],erro[i]);
        }
        getch();
        }
        
        for (i=1;i<=nv && iteracoes;i++) { 
          solucaoAnterior[i]=Yn[i][nv+1]; /*atualiza a solucao anterior */
        }

        MontaEstampaDC(0);
        if (ResolverSistemaR()) maiorErro=erro[1]=-1;
    }
    for (i=1;i<=nv && iteracoes;i++) {
        if (i==1) erro[i]=maiorErro=(Yn[i][nv+1]>TOLERANCIA)? fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]):fabs(solucaoAnterior[i]-Yn[i][nv+1]); /* Primeiro erro conferindo relativo ou absoluto */
        else if (Yn[i][nv+1]>TOLERANCIA) {  /* Se o valor e grande usa erro relativo*/
          maiorErro= fabs((Yn[i][nv+1]-solucaoAnterior[i])/Yn[i][nv+1])>maiorErro? fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]):maiorErro; /* Salva o maior erro */
          erro[i]=fabs((Yn[i][nv+1]-solucaoAnterior[i])/solucaoAnterior[i]);
        }
        else {  /* Se o valor e pequeno usa erro absoluto */
          maiorErro= fabs(solucaoAnterior[i]-Yn[i][nv+1])>maiorErro? fabs(solucaoAnterior[i]-Yn[i][nv+1]):maiorErro;
          erro[i]=fabs(solucaoAnterior[i]-Yn[i][nv+1]);
        }
      }   

  }
  if (fabs(maiorErro)>TOLERANCIA) {
    printf("Nao Convergiu\n");
  return 1;
  }
  return 0;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Abrir1Click(TObject *Sender)
{
 tolg=TOLGINICIAL; resistenciaDC=RESISTENCIADC; tolerancia=TOLERANCIA;
 frequenciaInicial=1; frequenciaFinal=1000; escala[0]='D'; npontos=1000;
 denovo:
  /* Leitura do netlist */
  ne=0; nv=0; strcpy(lista[0],"0");
  //for (i=0; i<=MAX_NOS;i++) strcpy(lista[i],"0");
  if (!OpenDialog1->Execute()) return;
  strcpy(nomeArquivo,OpenDialog1->FileName.c_str());
  arquivo=fopen(nomeArquivo,"r");
  if (arquivo==0) {
    printf("Arquivo %s inexistente",nomeArquivo);
    goto denovo;
  }
  printf("Lendo netlist:");
  fgets(txt,MAX_LINHA,arquivo);
  printf("T�tulo: %s",txt);
  while (fgets(txt,MAX_LINHA,arquivo)) {
    ne++; /* N�o usa o netlist[0] */
    if (ne>MAX_ELEM) {
      printf("O programa so aceita at� %d elementos",MAX_ELEM);
      return;
    }
    txt[0]=toupper(txt[0]);
    tipo=txt[0];
    sscanf(txt,"%10s",netlist[ne].nome);
    p=txt+strlen(netlist[ne].nome); /* In�cio dos par�metros */
    /* O que � lido depende do tipo */
    if (tipo=='R' || tipo=='C' || tipo=='L') {
      sscanf(p,"%10s%10s%lg",na,nb,&netlist[ne].valor);
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
      if(Numero(na,&netlist[ne].a)) return;
      if(Numero(nb,&netlist[ne].b)) return;
    }
    else if (tipo=='I' || tipo=='V') {
      sscanf(p,"%10s%10s%lg%lg%lg",na,nb,&netlist[ne].valor,&netlist[ne].fi,&netlist[ne].vt); /* usa valor para guardar o modulo, fi para fase e vt para DC */
      printf("%s %s %s %g %g %g\n",netlist[ne].nome,na,nb,netlist[ne].valor,netlist[ne].fi,netlist[ne].vt);
      if(Numero(na,&netlist[ne].a)) return;
      if(Numero(nb,&netlist[ne].b)) return;
    }
    else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
      sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[ne].valor);
      printf("%s %s %s %s %s %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor);
      if(Numero(na,&netlist[ne].a)) return;
      if(Numero(nb,&netlist[ne].b)) return;
      if(Numero(nc,&netlist[ne].c)) return;
      if(Numero(nd,&netlist[ne].d)) return;
    }
    else if (tipo=='O') { 
      sscanf(p,"%10s%10s%10s%10s",na,nb,nc,nd);
      printf("%s %s %s %s %s\n",netlist[ne].nome,na,nb,nc,nd);
      if(Numero(na,&netlist[ne].a)) return;
      if(Numero(nb,&netlist[ne].b)) return;
      if(Numero(nc,&netlist[ne].c)) return;
      if(Numero(nd,&netlist[ne].d)) return;
    }
    else if (tipo=='K') {
      sscanf(p,"%10s%10s%lg",indutorA,indutorB,&netlist[ne].valor);
      if (LocalizaIndutores(indutorA,&netlist[ne].a)) return; /* Aproveita que a variavel nao e usada pra salvar a posicao do indutor A  no netlist */
      if (LocalizaIndutores(indutorB,&netlist[ne].b)) return;
      printf("%s %s %s %g %d %d\n",netlist[ne].nome,indutorA,indutorB,netlist[ne].valor,netlist[ne].a,netlist[ne].b);
    }
    else if (tipo=='M') {
      temTransistor=1;
      netlist[i].x=0; /*usa a variavel x pra acompanhar se o transistor inverteu */
      sscanf(p,"%10s%10s%10s%10s%10s L=%lg W=%lg%lg%lg%lg%lg%lg%lg",na,nb,nc,nd,netlist[ne].npmos,&netlist[ne].comprimento,
      &netlist[ne].largura,&netlist[ne].k,&netlist[ne].vt,&netlist[ne].lambda,&netlist[ne].gamma,&netlist[ne].fi,&netlist[ne].ld);
      netlist[ne].npmos[0]=toupper(netlist[ne].npmos[0]);
      if(Numero(na,&netlist[ne].a)) return;
      if(Numero(nb,&netlist[ne].b)) return;
      if(Numero(nc,&netlist[ne].c)) return;
      if(Numero(nd,&netlist[ne].d)) return;
      printf("%s %d %d %d %d %s (L=)%lg (W=)%lg %lg %lg %lg %lg %lg %lg\n",netlist[ne].nome,netlist[ne].a,netlist[ne].b,netlist[ne].c,netlist[ne].d,netlist[ne].npmos,netlist[ne].comprimento,
      netlist[ne].largura,netlist[ne].k,netlist[ne].vt,netlist[ne].lambda,netlist[ne].gamma,netlist[ne].fi,netlist[ne].ld);
    }
    else if (tipo=='.') { /* Parametros come�a com .AC */
      if (!strcmp(netlist[ne].nome,".AC")) {
      sscanf(p,"%10s%d%lg%lg",escala,&npontos,&frequenciaInicial,&frequenciaFinal);
      if (frequenciaInicial>frequenciaFinal) {
        g=frequenciaInicial;
        frequenciaInicial=frequenciaFinal;
        frequenciaFinal=g;
      }
      printf("Configuracoes de escala: %s %d %g %g\n",escala,npontos,frequenciaInicial,frequenciaFinal);
      ne--;
      temAc=1;
      }
    }
    else if (tipo=='*') { /* Comentario comeca com "*" */
      printf("Comentario: %s",txt);
      ne--;
    }
    else {
      printf("Elemento desconhecido: %s\n",txt);
      getch();
      return;
    }
  }
  fclose(arquivo);
  /* Acrescenta vari�veis de corrente acima dos n�s, anotando no netlist */
  nn=nv;
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O' || tipo=='L') {
      nv++;
      if (nv>MAX_NOS) {
        printf("As correntes extra excederam o n�mero de vari�veis permitido (%d)",MAX_NOS);
        exit(1);
      }
      strcpy(lista[nv],"j"); /* Tem espa�o para mais dois caracteres */
      strcat(lista[nv],netlist[i].nome);
      netlist[i].x=nv;
    }
    else if (tipo=='H') {
      nv=nv+2;
      if (nv>MAX_NOS) {
        printf("As correntes extra excederam o n�mero de vari�veis permitido (%d)",MAX_NOS);
        exit(1);
      }
      strcpy(lista[nv-1],"jx"); strcat(lista[nv-1],netlist[i].nome);
      netlist[i].x=nv-1;
      strcpy(lista[nv],"jy"); strcat(lista[nv],netlist[i].nome);
      netlist[i].y=nv;
    }
  }
  /* Lista tudo */
  printf("Vari�veis internas: ");
  for (i=0; i<=nv; i++)
    printf("%d -> %s",i,lista[i]);
  if (mostraEstampa) {
	  printf("\nNetlist interno final\n");
	  for (i=1; i<=ne; i++) {
		tipo=netlist[i].nome[0];
		if (tipo=='R' || tipo=='C' || tipo=='L') {
		  printf("%s %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor);
		}
		else if (tipo=='I' || tipo=='V') {
		  printf("%s %d %d %g %g %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor,netlist[i].fi,netlist[i].vt);
		}
		else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
		  printf("%s %d %d %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor);
		}
		else if (tipo=='O') {
		  printf("%s %d %d %d %d\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d);
		}
		else if (tipo=='M') {
		  printf("%s %d %d %d %d %s %lg %lg %lg %lg %lg %lg %lg %lg\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].npmos,netlist[i].comprimento,
		  netlist[i].largura,netlist[i].k,netlist[i].vt,netlist[i].lambda,netlist[i].gamma,netlist[i].fi,netlist[i].ld);
		}
		if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O')
		  printf("Corrente jx: %d\n",netlist[i].x);
		else if (tipo=='H')
		  printf("Correntes jx e jy: %d, %d\n",netlist[i].x,netlist[i].y);
	  }
  }
  if(!temAc) {
    printf("Linha .AC com especificacoes da frequencia nao foi detectada.\nSerao utilizadas as frequencias do menu avan�ado\n");
    temAc=!temAc;
  }
  /* Resolve o sistema */
  if (!temTransistor) {
        MontaEstampaDC(0);
        if (ResolverSistemaR()) return;
		/* Mostra o sistema resolvido */
		if (mostraEstampa) {
		  printf("Sistema resolvido:");
			for (k=1; k<=nv; k++) {
			  strcpy(buf,"");
			  for (j=1; j<=nv+1; j++) {
				if (Yn[k][j]!=0) sprintf(tmp,"%+6.2f ",Yn[k][j]);
				else sprintf(tmp,"...... ");
				strcat(buf,tmp);
			  }
			  printf(buf);
			}
		}
        if (InicializarArquivos(temAc)) return;
        if (npontos-1){
            if (escala[0]=='L') {
              passoFrequencia=(frequenciaFinal-frequenciaInicial)/(npontos-1);
            }
            else if (escala[0]=='D') { 
              passoFrequencia=pow(10,(1.0/(npontos-1)));
            }
            else { /* Nao confere se for outra coisa qualquer, pressupoe OCT */
              passoFrequencia=pow(2,(1.0/(npontos-1)));
            }
        }
        else {
            if (escala[0]=='L') {
              passoFrequencia=(frequenciaFinal-frequenciaInicial);
            }
            else if (escala[0]=='D') { 
              passoFrequencia=10;
            }
            else { /* Nao confere se for outra coisa qualquer, pressupoe OCT */
              passoFrequencia=2;
            }
        }
        while (frequenciaInicial<=frequenciaFinal) {
          MontaEstampaAC();
          if (ResolverSistemaC()) {
            printf("\nSistema Singular\n");
            return;
          }
          GravarLinha();
          if (escala[0]=='L') {
            frequenciaInicial+=passoFrequencia;
          }
          else { 
            frequenciaInicial*=passoFrequencia;
          }
        }
      }
  else {
        if (ResolverNewtonRaphson()) return;
        if (InicializarArquivos(temAc)) return;
        if (npontos-1){
            if (escala[0]=='L') {
              passoFrequencia=(frequenciaFinal-frequenciaInicial)/(npontos-1);
            }
            else if (escala[0]=='D') { 
              passoFrequencia=pow(10,(1.0/(npontos-1)));
            }
            else { /* Nao confere se for outra coisa qualquer, pressupoe OCT */
              passoFrequencia=pow(2,(1.0/(npontos-1)));
            }
        }
        else {
            if (escala[0]=='L') {
              passoFrequencia=(frequenciaFinal-frequenciaInicial);
            }
            else if (escala[0]=='D') { 
              passoFrequencia=10;
            }
            else { /* Nao confere se for outra coisa qualquer, pressupoe OCT */
              passoFrequencia=2;
            }
        }
        while (frequenciaInicial<=frequenciaFinal) {
          MontaEstampaAC();
          if (ResolverSistemaC()) {
            printf("\nSistema Singular\n");
            return;
          }
          GravarLinha();
          if (escala[0]=='L') {
            frequenciaInicial+=passoFrequencia;
          }
          else { 
            frequenciaInicial*=passoFrequencia;
          }
        }
    }
  if (temAc) {
  fclose(arquivo);
  printf("Arquivo %s criado com a tabela da resposta em frequencia\n",nomeArquivo);
  }
  if (temTransistor) printf("Foram feitas %d iteracoes e %d randomizacoes no ponto de operacao\n",iteracoes-1,tentativas-1);
  return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  printf("Vers�o %s",versao);
  return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MostrarparametrosMOS1Click(TObject *Sender)
{
  MostrarparametrosMOS1->Checked=!MostrarparametrosMOS1->Checked;
  mostraMOS=MostrarparametrosMOS1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::IniciarMOSconduzindo1Click(TObject *Sender)
{
  IniciarMOSconduzindo1->Checked=!IniciarMOSconduzindo1->Checked;
  iniciaConduzindo=IniciarMOSconduzindo1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MostrarEstampasAC1Click(TObject *Sender)
{
  MostrarEstampasAC1->Checked=!MostrarEstampasAC1->Checked;
  mostraEstampaAC=MostrarEstampasAC1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MostrarConvergencia1Click(TObject *Sender)
{
  MostrarConvergencia1->Checked=!MostrarConvergencia1->Checked;
  mostraNP=MostrarConvergencia1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MostrarEstampas2Click(TObject *Sender)
{
  MostrarEstampas2->Checked=!MostrarEstampas2->Checked;
  mostraEstampa=MostrarEstampas2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Avanado1Click(TObject *Sender)
{
 Form2->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::TOLGChange(TObject *Sender)
{
  tolg=TOLG->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::resistenciaDCTextoChange(TObject *Sender)
{
  resistenciaDC=resistenciaDCTexto->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::toleranciaTextoChange(TObject *Sender)
{
  tolerancia=toleranciaTexto->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::frequenciaInicialTextoChange(TObject *Sender)
{
  frequenciaInicial=frequenciaInicialTexto->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::frequenciaFinalTextoChange(TObject *Sender)
{
  frequenciaFinal=frequenciaFinalTexto->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::npontosTextoChange(TObject *Sender)
{
  npontos=npontosTexto->Text.ToDouble();    
}
//---------------------------------------------------------------------------
void __fastcall TForm2::escalaListaChange(TObject *Sender)
{
  if(escalaLista->ItemIndex == 0) escala[0]='D';
  else if (escalaLista->ItemIndex == 1) escala[0]='L';
  else if (escalaLista->ItemIndex == 2) escala[0]='O';
}
//---------------------------------------------------------------------------


