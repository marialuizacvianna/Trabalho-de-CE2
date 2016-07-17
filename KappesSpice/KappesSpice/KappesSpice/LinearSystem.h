#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <complex>
#include <cmath>
#include "Componente.h"

typedef complex<double> dcomp;
using namespace std;

class LinearSystem
{
	public:
		void setRowsValue(unsigned numberOfNodes);
		void InitializeG_Matrix(); 
		void PrintG_Matrix();
		void SolveLinearSystem(); //G_Matrix is almost a square Matrix, it's a A[i][i+1]. In the last column we hold the b for the Ax = b system. So we just need to define the number of rows.
		int GetRows();
		void PrintVariables();
		void SaveDC();
		void ResetG_Matrix();
		double ** G_Matrix;

		dcomp **G_MatrixAC; // complex MATRIX
		void InitializeG_MatrixAC();
		void ResetG_MatrixAC();
		void SolveLinearSystemC();
		vector<dcomp> variablesComp;

		int extraRows;
		vector<double> lastVariables;
		vector<double> variables;
		vector<double> DCvariables;
		int rows;
		vector<double> error;
		double maxError;
	private:

};
