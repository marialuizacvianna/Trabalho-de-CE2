#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <complex>
#include "Componente.h"

#define  PI2 		(8 * atan(1.0))
#define PI 			(4 * atan(1.0))
#define I			dcomp(0.0, 1.0)
#define G_180       180

#define TOLG 1e-16

typedef std::complex<double> dcomp;

using namespace std;

class LinearSystem
{
	public:
		void setRowsValue(unsigned numberOfNodes);
		void InitializeG_Matrix(); 
		void PrintG_Matrix();
		int SolveLinearSystem(); //G_Matrix is almost a square Matrix, it's a A[i][i+1]. In the last column we hold the b for the Ax = b system. So we just need to define the number of rows.
		int GetRows();
		void PrintVariables();
		void SaveDC();
		void ResetG_Matrix();
		double ** G_Matrix;

		dcomp **G_MatrixAC; // complex MATRIX
		void InitializeG_MatrixAC();
		void ResetG_MatrixAC();
		int SolveLinearSystemC();
		void PrintG_MatrixAC();

		vector<dcomp> variablesComp;

		int extraRows;
		vector<string> extraRowsName;
		vector<double> lastVariables;
		vector<double> variables;
		vector<double> DCvariables;
		int rows;
		vector<double> error;
		double maxError;
	private:

};
