#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "Componente.h"
using namespace std;

class LinearSystem
{
	public:
		//void setRowsValue(vector <Componente *>);
		void setRowsValue(unsigned numberOfNodes);
		void InitializeG_Matrix(); 
		void PrintG_Matrix();
		void SolveLinearSystem(); //G_Matrix is almost a square Matrix, it's a A[i][i+1]. In the last column we hold the b for the Ax = b system. So we just need to define the number of rows.
		int GetRows();
		void PrintVariables();
		void SaveDC_Matrix();
		void ResetG_Matrix();
		double ** G_Matrix;
		double ** DC_Matrix;
		int extraRows;
	private:
		int rows;
		vector<double> variables;
};
