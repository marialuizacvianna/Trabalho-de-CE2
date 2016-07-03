#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
using namespace std;

class LinearSystem
{
	public:
		void InitializeG_Matrix(); //G_Matrix is a square Matrix, so we just need to define the number of rows
		void PrintG_Matrix();
		double ** G_Matrix;
		unsigned rows;
};
