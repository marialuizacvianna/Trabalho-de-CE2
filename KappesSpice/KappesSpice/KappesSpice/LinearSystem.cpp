#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <iomanip>
#include <complex>
#include <cmath>
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
		lastVariables.push_back(0.1);
		variables.push_back(0);
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


void LinearSystem::setRowsValue(unsigned numberOfnodes)
{
	rows = numberOfnodes;
}

/*void LinearSystem::SolveLinearSystem()
{
	int n = (rows + extraRows);
	for (int i = 0; i<n; i++) {
		// Search for maximum in this column
		double maxEl = abs(G_Matrix[i][i]);
		int maxRow = i;
		for (int k = i + 1; k<n; k++) {
			if (abs(G_Matrix[k][i]) > maxEl) {
				maxEl = abs(G_Matrix[k][i]);
				maxRow = k;
			}
		}

		// Swap maximum row with current row (column by column)
		for (int k = i; k<n + 1; k++) {
			double tmp = G_Matrix[maxRow][k];
			G_Matrix[maxRow][k] = G_Matrix[i][k];
			G_Matrix[i][k] = tmp;
		}

		// Make all rows below this one 0 in current column
		for (int k = i + 1; k<n; k++) {
			double c = -G_Matrix[k][i] / G_Matrix[i][i];
			for (int j = i; j<n + 1; j++) {
				if (i == j) {
					G_Matrix[k][j] = 0;
				}
				else {
					G_Matrix[k][j] += c * G_Matrix[i][j];
				}
			}
		}
	}

	// Solve equation Ax=b for an upper triangular matrix A
	vector<double> x(n);
	for (int i = n - 1; i >= 0; i--) {
		x[i] = G_Matrix[i][n] / G_Matrix[i][i];
		for (int k = i - 1; k >= 0; k--) {
			G_Matrix[k][n] -= G_Matrix[k][i] * x[i];
		}
	}
	this->variables = x;

}*/


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
		cout << "Tensao "<< i <<": " << variables[i] <<endl; 
	for (; i < (rows + extraRows); i++)
		cout <<  "Corrente " << i << ": " << variables[i] << endl;
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
void LinearSystem::SolveLinearSystemC() 
{
	int n = (rows + extraRows);
	for (int i = 0; i<n; i++) {
		// Search for maximum in this column
		dcomp maxEl = abs(G_MatrixAC[i][i]);
		int maxRow = i;
		for (int k = i + 1; k<n; k++) {
			if (abs(G_MatrixAC[k][i]) > abs(maxEl) ){
				maxEl = abs(G_MatrixAC[k][i]);
				maxRow = k;
			}
		}

		// Swap maximum row with current row (column by column)
		for (int k = i; k<n + 1; k++) {
			dcomp tmp = G_MatrixAC[maxRow][k];
			G_MatrixAC[maxRow][k] = G_MatrixAC[i][k];
			G_MatrixAC[i][k] = tmp;
		}

		// Make all rows below this one 0 in current column
		for (int k = i + 1; k<n; k++) {
			dcomp c = -G_MatrixAC[k][i] / G_MatrixAC[i][i];
			for (int j = i; j<n + 1; j++) {
				if (i == j) {
					G_MatrixAC[k][j] = 0;
				}
				else {
					G_MatrixAC[k][j] += c * G_MatrixAC[i][j];
				}
			}
		}
	}

	// Solve equation Ax=b for an upper triangular matrix A
	vector<dcomp> x(n);
	for (int i = n - 1; i >= 0; i--) {
		x[i] = G_MatrixAC[i][n] / G_MatrixAC[i][i];
		for (int k = i - 1; k >= 0; k--) {
			G_MatrixAC[k][n] -= G_MatrixAC[k][i] * x[i];
		}
	}
	this->variablesComp = x;


}
