#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "LinearSystem.h"
using namespace std;


void LinearSystem::InitializeG_Matrix()
{
	G_Matrix = new double*[rows]; //Creating the rows, a array of pointers to double.
	for (int i = 0; i < rows; i++)
		G_Matrix[i] = new double[rows]; //Creating the columns, the array of double
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < rows; j++)
			G_Matrix[i][j] = 0;
}

void LinearSystem::PrintG_Matrix()
{
	cout << endl;

	for (int i = 0; i < rows; i++)
	{
		cout << "| ";
		for (int j = 0; j < rows; j++)
		{
			cout << G_Matrix[i][j];
			cout << " ";
		}
		cout << "|" << endl;
	}

	cout << endl;
}
