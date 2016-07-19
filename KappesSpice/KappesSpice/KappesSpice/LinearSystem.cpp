#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <iomanip>
#include <complex>
#include "LinearSystem.h"

#define PRINT_WIDTH 5		

using namespace std;



void LinearSystem::InitializeG_Matrix()
{
	G_Matrix = new double*[(rows+extraRows)]; //Creating the rows, a array of pointers to double.
	for (int i = 0; i < (rows+ extraRows); i++)
		G_Matrix[i] = new double[(rows+ extraRows) + 1]; //Creating the columns, the array of double
	for (int i = 0; i < (rows + extraRows); i++) //reseting G_Matrix and lastVariables
	{
		if (i == 0)
			lastVariables.push_back(0);
		lastVariables.push_back(0.1);
		variables.push_back(0);
		variablesComp.push_back((0.0, 0.0));
		error.push_back(0);
		for (int j = 0; j < (rows + extraRows) + 1; j++)
			G_Matrix[i][j] = 0;
	}
}

void LinearSystem::PrintG_Matrix()
{
	cout << endl;

	for (int i = 1; i < (rows + extraRows); i++) //i starts from 1, becouse we ignore the node 0( AKA ground)
	{
		cout << "| ";
		for (int j = 1; j < (rows + extraRows + 1); j++) //j starts from 1, becouse we ignore the node 0( AKA ground)
		{
			if (G_Matrix[i][j])
				cout  << setw(PRINT_WIDTH) << G_Matrix[i][j];
			else
				cout << setw(PRINT_WIDTH) << ".....";
			cout << " ";
		}
		cout << "|" << endl;
	}

	cout << endl;
}


void LinearSystem::PrintG_MatrixAC()
{
	cout << endl;

	for (int i = 1; i < (rows + extraRows); i++) //i starts from 1, becouse we ignore the node 0( AKA ground)
	{
		cout << "| ";
		for (int j = 1; j < (rows + extraRows + 1); j++) //j starts from 1, becouse we ignore the node 0( AKA ground)
		{
			if (G_MatrixAC[i][j] == (0.0,0.0))
				cout << setw(PRINT_WIDTH) << "..........";
			else
				cout << setw(PRINT_WIDTH) << G_MatrixAC[i][j];
			cout << " ";
		}
		cout << "|" << endl;
	}

	cout << endl;
}

void LinearSystem::setRowsValue(unsigned numberOfnodes)
{
	rows = numberOfnodes;
}


int LinearSystem::SolveLinearSystem()
{
	int i, j, l, a;
	double t, p;
	int nv = (rows + extraRows - 1);

	for (i = 1; i <= nv ; i++) {
		t = 0.0;
		a = i;
		for (l = i; l <= nv; l++) {
			if (fabs(G_Matrix[l][i])>fabs(t)) {
				a = l;
				t = G_Matrix[l][i];
			}
		}
		if (i != a) {
			for (l = 1; l <= nv + 1; l++) {
				p = G_Matrix[i][l];
				G_Matrix[i][l] = G_Matrix[a][l];
				G_Matrix[a][l] = p;
			}
		}
		if (fabs(t)<TOLG) {
			cout << "Sistema singular" << endl;
			return 1;
		}
		for (j = nv + 1; j>0; j--) {  /* Basta j>i em vez de j>0 */
			G_Matrix[i][j] = G_Matrix[i][j]/t;
			p = G_Matrix[i][j];
			if (p != 0)  /* Evita operacoes com zero */
				for (l = 1; l <= nv; l++) {
					if (l != i)
						G_Matrix[l][j] -= G_Matrix[l][i] * p;
				}
		}
	}
	
	for (i = 0; i <=nv ;i++)
		this->variables[i] = G_Matrix[i][nv + 1];
	return 0;
}




void LinearSystem::PrintVariables()
{

	int i = 1;
	for (; i < rows; i++)
		cout << "Tensao e" << i << ": " << variables[i] << endl;
	for (; i < (rows + extraRows); i++)
		cout << "Corrente "<< extraRowsName[i - rows] << ": " << variables[i] << endl;
	cout << endl;

}
int LinearSystem::GetRows()
{
	return rows - 1; // we dont recognize the 0 (AKA Ground)
}
void LinearSystem::SaveDC()
{
	for (unsigned i = 0; i < variables.size();i++)
		DCvariables.push_back(variables[i]);
}

void LinearSystem::ResetG_Matrix()
{
	for (int i = 0; i < (rows + extraRows); i++)
		for (int j = 0; j < (rows + extraRows) + 1; j++)
			G_Matrix[i][j] = 0;

}




void LinearSystem::InitializeG_MatrixAC()
{
	G_MatrixAC = new dcomp*[(rows + extraRows)]; //Creating the rows, a array of pointers to double.
	for (int i = 0; i < (rows + extraRows); i++)
		G_MatrixAC[i] = new dcomp[(rows + extraRows) + 1]; //Creating the columns, the array of double
	for (int i = 0; i < (rows + extraRows); i++) //reseting G_Matrix and lastVariables
		for (int j = 0; j < (rows + extraRows) + 1; j++)
			G_MatrixAC[i][j] = 0;

}

void LinearSystem::ResetG_MatrixAC()
{
	for (int i = 0; i < (rows + extraRows); i++)
		for (int j = 0; j < (rows + extraRows) + 1; j++)
			G_MatrixAC[i][j] = 0;

}



int LinearSystem::SolveLinearSystemC()
{
	int i, j, l, a;
	dcomp t, p;
	int nv = (rows + extraRows - 1);

	for (i = 1; i <= nv; i++) {
		t = 0.0 + 0.0*I;
		a = i;
		for (l = i; l <= nv; l++) {
			if (abs(G_MatrixAC[l][i])>abs(t)) {
				a = l;
				t = G_MatrixAC[l][i];
			}
		}
		if (i != a) {
			for (l = 1; l <= nv + 1; l++) {
				p = G_MatrixAC[i][l];
				G_MatrixAC[i][l] = G_MatrixAC[a][l];
				G_MatrixAC[a][l] = p;
			}
		}
		if (abs(t)<TOLG) {
			cout << "Sistema singular" << endl;
			return 1;
		}
		for (j = nv + 1; j>0; j--) {  /* Basta j>i em vez de j>0 */
			G_MatrixAC[i][j] = G_MatrixAC[i][j] / t;
			p = G_MatrixAC[i][j];
			if (p != (0.0,0.0))  /* Evita operacoes com zero */
				for (l = 1; l <= nv; l++) {
					if (l != i)
						G_MatrixAC[l][j] -= G_MatrixAC[l][i] * p;
				}
		}
	}

	for (i = 0; i <= nv;i++)
		this->variablesComp[i] = G_MatrixAC[i][nv + 1];
	return 0;
}