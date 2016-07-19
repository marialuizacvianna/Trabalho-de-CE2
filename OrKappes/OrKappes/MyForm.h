#pragma once
#include <msclr\marshal_cppstd.h>
#include "Componente.h"
#include "Netlist.h"
#include "LinearSystem.h"
std::string buffer;

namespace OrKappes {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	protected:
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  arquivoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  abrirToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog3;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog4;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog5;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog6;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog7;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::ComponentModel::IContainer^  components;
	 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->arquivoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->abrirToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip2 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog3 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog4 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog5 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog6 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog7 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->arquivoToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(874, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// arquivoToolStripMenuItem
			// 
			this->arquivoToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->abrirToolStripMenuItem });
			this->arquivoToolStripMenuItem->Name = L"arquivoToolStripMenuItem";
			this->arquivoToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->arquivoToolStripMenuItem->Text = L"Arquivo";
			// 
			// abrirToolStripMenuItem
			// 
			this->abrirToolStripMenuItem->Name = L"abrirToolStripMenuItem";
			this->abrirToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->abrirToolStripMenuItem->Text = L"Abrir";
			this->abrirToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::abrirToolStripMenuItem_Click);
			// 
			// contextMenuStrip2
			// 
			this->contextMenuStrip2->Name = L"contextMenuStrip2";
			this->contextMenuStrip2->Size = System::Drawing::Size(61, 4);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// openFileDialog3
			// 
			this->openFileDialog3->FileName = L"openFileDialog3";
			// 
			// openFileDialog4
			// 
			this->openFileDialog4->FileName = L"openFileDialog4";
			// 
			// openFileDialog5
			// 
			this->openFileDialog5->FileName = L"openFileDialog5";
			// 
			// openFileDialog6
			// 
			this->openFileDialog6->FileName = L"openFileDialog6";
			// 
			// openFileDialog7
			// 
			this->openFileDialog7->FileName = L"openFileDialog7";
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 27);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(850, 492);
			this->richTextBox1->TabIndex = 2;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(874, 531);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::Color::CornflowerBlue;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"OrKappes";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void abrirToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {


		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{

			String^ buff;
			//const char* netlistPath = "C:\\Trabalho-de-CE2\\simples.net";
			//const char* netlistPath = openFileDialog1->FileName;
			Netlist netlist(msclr::interop::marshal_as<std::string>(openFileDialog1->FileName));
			richTextBox1->AppendText("Fez o Netlist...\n");
			netlist.SistemaLinear.setRowsValue(netlist.GetNumberOfNodes());
			netlist.SistemaLinear.InitializeG_Matrix();
			//netlist.DoConductanceMatrixDC();
			//cout << "FEZ A MATRIZ" << endl;
			//netlist.SistemaLinear.PrintG_Matrix();
			if (netlist.NewtonRaphson()) {
				richTextBox1->AppendText("Nao convergiu...\n");
				return;
			}
			
		richTextBox1->AppendText("Achou o Ponto de Operacao...\n");
		//netlist.SistemaLinear.PrintG_Matrix();

		int i = 1;
		for (; i < netlist.SistemaLinear.rows; i++)
		{
			richTextBox1->AppendText("Tensao ");
			buff = gcnew String(to_string(i).c_str());
			richTextBox1->AppendText(buff);
			richTextBox1->AppendText(" :");
			buff = gcnew String(to_string(netlist.SistemaLinear.variables[i]).c_str());
			richTextBox1->AppendText(buff);
			richTextBox1->AppendText("\n");

		}

		for (; i < (netlist.SistemaLinear.rows + netlist.SistemaLinear.extraRows); i++)
		{
			richTextBox1->AppendText("Corrente ");
			buff = gcnew String(netlist.SistemaLinear.extraRowsName[i - netlist.SistemaLinear.rows].c_str());
			richTextBox1->AppendText(buff);
			richTextBox1->AppendText(" :");
			buff = gcnew String(to_string(netlist.SistemaLinear.variables[i]).c_str());
			richTextBox1->AppendText(buff);
			richTextBox1->AppendText("\n");

		}

		netlist.SistemaLinear.PrintVariables();
		netlist.WriteDCData();
		netlist.ACSweep();
		//system("pause");
		richTextBox1->AppendText("Finalizou o programa!...\n");
		}
		return;

	}

	public: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
};
}
