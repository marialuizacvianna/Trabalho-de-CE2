import matplotlib.pyplot as plt
import numpy as np
from Tkinter import *

endereco = './output.txt'


results =[]
numberVariables = 0
variables = []

def convertFile(link):

	global variables

	line_number = 0

	Simulacao = open(link, 'r') 

	variables = Simulacao.readline().replace(' ','').split('\t')

	for Simulacao_line in Simulacao:

		results.append(Simulacao_line.replace(' ','').split('\t'))
		del results[-1][-1]
        
        line_number += 1   

	for i in range(len(results)):
 		for j in range(len(results[i])):
			results[i][j] = float(results[i][j])
			
	Simulacao.close()

def showSelectionMenu(variables):
	global window
	window = Tk()

	window.geometry("%dx%d+%d+%d" % (330, 80, 200, 150))
	window.title("Escolher Variavel")
	global var
	var = StringVar(window)

	var.set(variables[1])
	global choices

	choices = variables
	
	option = OptionMenu(window, var, *choices)
	option.pack(side='left', padx=10, pady=10)
	button = Button(window, text="Escolher Variavel Para Plotar", command=select)
	button.pack(side='left', padx=20, pady=10)
	window.mainloop()


def select():
	window.quit()
	
	global variavelSelecionada
	global variavelPlotar
	variavelSelecionada = var.get()
	
	variavelPlotar = choices.index(variavelSelecionada)

	

def printResultsTable():
	for i in range(len(results)):
			print results[i]


		
def plotResults(variavel):
	x = [row[0] for row in results]
	y = [row[variavel] for row in results]
	plt.plot(x, y)
	plt.show()

convertFile(endereco)
numberVariables = len(variables) - 1
showSelectionMenu(variables)
plotResults(variavelPlotar)


