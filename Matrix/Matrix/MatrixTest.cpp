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

int main(int argc, char* argv[])
{
	LinearSystem matriz;
	matriz.rows = 3;
	matriz.InitializeG_Matrix();
	matriz.G_Matrix[0][1] = 1;
	matriz.PrintG_Matrix();

	system("pause");

	return 0;
}
