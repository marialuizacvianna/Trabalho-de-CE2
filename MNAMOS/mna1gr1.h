//---------------------------------------------------------------------------

#ifndef mna1gr1H
#define mna1gr1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TMemo *Memo1;
        TMainMenu *MainMenu1;
        TMenuItem *Arquivo1;
        TMenuItem *Abrir1;
        TMenuItem *Opes1;
        TMenuItem *MostrarEstampasAC1;
        TMenuItem *MostrarConvergencia1;
        TMenuItem *MostrarparametrosMOS1;
        TMenuItem *IniciarMOSconduzindo1;
        TMenuItem *MostrarEstampas2;
        TMenuItem *Avanado1;
        void __fastcall Abrir1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall MostrarEstampasAC1Click(TObject *Sender);
        void __fastcall MostrarparametrosMOS1Click(TObject *Sender);
        void __fastcall IniciarMOSconduzindo1Click(TObject *Sender);
        void __fastcall MostrarConvergencia1Click(TObject *Sender);
        void __fastcall MostrarEstampas2Click(TObject *Sender);
        void __fastcall Avanado1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
