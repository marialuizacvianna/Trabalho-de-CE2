#include "stdafx.h"
#include "Solver.h"
#include "SimulationParameters.h"
//#include "cvplot.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
using namespace std;

double operator-(vector<double> a, vector<double>b)
{
	double r = 0;
	double m = 0;
	double div = 0;
	for (int i = 0; i < a.size(); i++) {
		div = (a[i] < 1) ? 1.0 : a[i];
		r += abs((a[i] - b[i]) / div);
		m = (r > m) ? r : m;
	}
	return m;
}
bool Solver::calculateNewtonRaphson()
{
	//Calcula o Erro, A fórmula está descrita no operador acima;
	double erro = x - lastX;
	//Calcula Pelo Menos Uma iteração
	if (simParameters.newtonIteratio == 0) {
		simParameters.newtonIteratio++;
		return true;
	}
	//Se não existem Componentes não Lineares não preciso iterar
	if (simParameters.nonLinearComponents == 0)
		return false;
	if (simParameters.nonConvergingIterations > 5)
	{
		stringstream error;
		error << "Erro de Convergencia no Circuito, por favor verifique os valores" << endl
			<< "Numero de Reinicialisacoes: " << simParameters.nonConvergingIterations << endl
			<< "Tempo de Simulacao: " << simParameters.actualStep * (simParameters.stepSize / simParameters.subSteps) << endl
			<< "Nos com problema de convergencia: ";
		vector<unsigned> nonConvergingNodes = getNonConvergingVariables();
		for (vector<unsigned>::iterator i = nonConvergingNodes.begin(); i != nonConvergingNodes.end(); i++) {
			error << _nodeNames[(*i)] << " ";
		}
		error << endl << endl;
		throw runtime_error(error.str());
	}
	//Verifica Se o Número Maximo de iterações do Newton Raphson passou

	if (simParameters.newtonIteratio > 100 && erro > simParameters.newtonTolerance)
	{
		simParameters.nonConvergingIterations++;
		for (vector<unsigned>::iterator i = simParameters.newtonNodes.begin(); i != simParameters.newtonNodes.end(); i++) {
				if ((*i) != 0) {
					x[(*i) - 1] = (rand() % 200) - 100;
				}
		}
		simParameters.newtonIteratio = 0;
		return true;
	}
	simParameters.newtonIteratio++;
	if (erro < simParameters.newtonTolerance) {
		simParameters.newtonIteratio = 0;
		return false;
	}
	lastX = x;
	return true;
	

}

vector<unsigned> Solver::getNonConvergingVariables()
{
	vector<unsigned> nonConvergingNodes;
	nonConvergingNodes.resize(0);
	for (unsigned i = 0; i < simParameters.newtonNodes.size(); i++) {
		int j = (simParameters.newtonNodes[i] != 0) ? simParameters.newtonNodes[i] - 1 : 0;
		if (simParameters.newtonNodes[i] != 0) {
			double d = (x[j] < 1) ? 1 : x[j];
			if (abs(x[j] - lastX[j]) / d > simParameters.newtonTolerance)
				nonConvergingNodes.push_back(j);
		}
	}
	return nonConvergingNodes;
}

void Solver::TimeSimulation()
{
	srand(time(NULL));
	x.resize(numberOfVariables);
	lastX.resize(numberOfVariables);


	for (vector<double>::iterator a = lastX.begin(); a != lastX.end(); a++)
		(*a) = 5;

	for (vector<double>::iterator a = x.begin(); a != x.end(); a++)
		(*a) = 1000.0;

	// Calcular numero de iterações e criar arquivo que armazenam dados
	cout << endl << "Simulando..." << endl;
	outputFile = new ofstream(simParameters.outFile);

	if (outputFile->is_open()) {
		outputFile->clear();
		//Cria Header do Arquivo de Saida
		*outputFile << setw(12) << "t" << '\t';
		for (int i = 0; i < _nodeNames.size(); i++)
			*outputFile << setw(12) << _nodeNames[i] << '\t';
		*outputFile << endl;
		//Calcula o numero de iterações
		unsigned long maxIterations = simParameters.simulationTime / (simParameters.stepSize * simParameters.subSteps);
		//Seta Configurações Para calculo de Bias Point

		double stepSize = simParameters.stepSize;
		simParameters.stepSize = 1e-9 * (simParameters.stepSize * simParameters.subSteps);
		int adams = simParameters.adamsMethod;

		simParameters.adamsMethod = 1;
		for (vector<Component*>::iterator component = _components.begin(); component != _components.end(); component++)
			(*component)->updateValue(simParameters.actualStep);
		while (calculateNewtonRaphson()) {
			clearCondunctanceMatrix();
			addStamps();
			SolveConductanceMatrix();
			for (vector<Component*>::iterator component = _components.begin(); component != _components.end(); component++)
			{
				(*component)->postAnalisysCalculations(x);
				(*component)->addInitialCondition(x);
			}
		}
		simParameters.nonConvergingIterations = 0;
		//Retorna Valores Para calculo no tempo
		simParameters.adamsMethod = adams;
		simParameters.stepSize = stepSize;
		simParameters.newtonIteratio = 0;
		outputToFile();
		//Começa Simulação no tempo
		for (simParameters.actualStep = 1; simParameters.actualStep < maxIterations; simParameters.actualStep++)
		{
			//Faz o Update dos Valores de Fonte
			for (vector<Component*>::iterator component = _components.begin(); component != _components.end(); component++)
				(*component)->updateValue(simParameters.actualStep);
			while (calculateNewtonRaphson()) {
				clearCondunctanceMatrix();
				addStamps();
				SolveConductanceMatrix();
			}
			simParameters.nonConvergingIterations = 0;C:\
			simParameters.newtonIteratio = 0;
			outputToFile();
			for (int j = 0; j < _components.size(); j++)
			{
				_components[j]->postAnalisysCalculations(x);
			}
		}
		outputFile->close();
	}
	else {
		throw runtime_error("Nao Foi Possivel Abrir Seu Arquivo, tente novamente");
	}
	
}
void Solver::outputToFile() {
	*outputFile << setprecision(12) << setw(12) << simParameters.actualStep * simParameters.stepSize << '\t';
	for (int k = 0; k < x.size(); k++)
		*outputFile << setprecision(12) << setw(12) << x[k] << '\t';
	*outputFile << endl;
}
void Solver::addStamps()
{
	for (vector<Component*>::iterator component = _components.begin(); component != _components.end(); component++)
	{
		if ((*component)->nonLinear) {
			(*component)->addStamp(_conductanceMatrix, x, numberOfVariables);
		}
		else {
			(*component)->addStamp(_conductanceMatrix, numberOfVariables);
		}
	}
}

void Solver::clearCondunctanceMatrix() {
	for (int i = 0; i < numberOfVariables; i++)
	{
		for (int j = 0; j <numberOfVariables + 1; j++)
		{
			_conductanceMatrix[i][j] = 0.0;
		}
	}
}
void Solver::createAdjacenceMatrix()
{
	// inicialisa a matriz de adjacencia
	int rows = _nodes.size();
	int cols = _components.size();
	_adjacentMatrix = new char*[rows];
	for (int i = 0; i < rows; i++)
		_adjacentMatrix[i] = new char[rows * cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			_adjacentMatrix[i][j] = 0;
	for (size_t i = 0; i < cols; i++)
	{
		_adjacentMatrix[_components.at(i)->getNode(0)][i] = 1;
		_adjacentMatrix[_components.at(i)->getNode(1)][i] = -1;
	}

	//cout << endl << "Matriz de Adjacencia" << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
		{
			//cout << (int)_adjacentMatrix[i][j] << '\t';
		}
		//cout << endl;
	}

}

void Solver::createConductanceMatrix()
{

	//Calculate Number of Variables
	numberOfVariables = _nodes.size() - 1;
	for (int i = 1; i < numberOfVariables + 1; i++)
	{
		_nodeNames.push_back(to_string(i));
	}
	int adder;
	vector<string> names;

	for (int i = 0; i < _components.size(); i++)
	{
		adder = _components.at(i)->getNumberOfVariables();
		//cout << endl << _components.at(i)->getName() << " possui " << adder << " variaveis extras" << endl;
		names = _components.at(i)->getExtraVariablesNames();
		for (unsigned j = 0; j < adder; j++)
		{
			_nodeNames.push_back(names[j]);
			_components.at(i)->addExtraNodePosition(numberOfVariables + j);
		}
		numberOfVariables += adder;
	}
	//Inicialisa a Matriz GM
	int rows = numberOfVariables;
	int cols = numberOfVariables + 1;
	_conductanceMatrix = new double*[rows];
	for (int i = 0; i < rows; i++)
		_conductanceMatrix[i] = new double[rows * cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			_conductanceMatrix[i][j] = 0;
	
	rows = numberOfVariables ;
	cols = numberOfVariables + 1;

}

void Solver::printConductanceMatrix()
{
	cout << endl;
	for (int i = 0; i < numberOfVariables; i++) {
		cout << setw(1) << "| ";
		for (int j = 0; j < numberOfVariables; j++)
		{
			cout << setprecision(2) << setw(9) << _conductanceMatrix[i][j];
		}
		cout << setw(3) << "|" << setw(4) <<_conductanceMatrix[i][numberOfVariables] << "|" << endl;
	}
}


void Solver::SolveConductanceMatrix() {
	int n = numberOfVariables ;
	for (int i = 0; i<n; i++) {
		// Search for maximum in this column
		double maxEl = abs(_conductanceMatrix[i][i]);
		int maxRow = i;
		for (int k = i + 1; k<n; k++) {
			if (abs(_conductanceMatrix[k][i]) > maxEl) {
				maxEl = abs(_conductanceMatrix[k][i]);
				maxRow = k;
			}
		}

		// Swap maximum row with current row (column by column)
		for (int k = i; k<n + 1; k++) {
			double tmp = _conductanceMatrix[maxRow][k];
			_conductanceMatrix[maxRow][k] = _conductanceMatrix[i][k];
			_conductanceMatrix[i][k] = tmp;
		}

		// Make all rows below this one 0 in current column
		for (int k = i + 1; k<n; k++) {
			double c = -_conductanceMatrix[k][i] / _conductanceMatrix[i][i];
			for (int j = i; j<n + 1; j++) {
				if (i == j) {
					_conductanceMatrix[k][j] = 0;
				}
				else {
					_conductanceMatrix[k][j] += c * _conductanceMatrix[i][j];
				}
			}
		}
	}

	// Solve equation Ax=b for an upper triangular matrix A
	vector<double> x(n);
	for (int i = n - 1; i >= 0; i--) {
		x[i] = _conductanceMatrix[i][n] / _conductanceMatrix[i][i];
		for (int k = i - 1; k >= 0; k--) {
			_conductanceMatrix[k][n] -= _conductanceMatrix[k][i] * x[i];
		}
	}
	this->x = x;
}

void Solver::printResults()
{
	//cout << endl << "Resultados: " << endl;
	for (int i = 0; i < x.size(); i++)
	{
		//cout << _nodeNames[i] << " = " << x[i] << ";" << endl;
	}
}

void Solver::computeOpamp(Component * opamp)
{
	if (opamp->getNode(0) != 0 && opamp->getNode(1) != 0)
	{
		for (int i = 0; i < numberOfVariables + 1; i++)
		{
			_conductanceMatrix[opamp->getNode(0)-1][i] += _conductanceMatrix[opamp->getNode(1)-1][i];
			_conductanceMatrix[opamp->getNode(1)-1][i] += _conductanceMatrix[opamp->getNode(2)-1][i];
		}
	}
	if (opamp->getNode(2) != 0 && opamp->getNode(3) != 0)
	{
		for (int i = 0; i < numberOfVariables; i++)
		{
			_conductanceMatrix[i][opamp->getNode(2)-1] += _conductanceMatrix[i][opamp->getNode(3)-1];
			_conductanceMatrix[i][opamp->getNode(3)-1] += _conductanceMatrix[i][opamp->getNode(2)-1];
		}
	}
}

Solver::Solver(Netlist & net) : net(net)
{
	_components = net.getComponents();
	_nodes = net.getNodes();
}

Solver::~Solver()
{
}
