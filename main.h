//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "pies.h"
#include <inifiles.hpp>
#include <ExtCtrls.hpp> // Подключение библиотеки
//---------------------------------------------------------------------------
class TmainForm : public TForm
{
__published:	// IDE-managed Components
	TPie *pie;
	TButton *Button1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
	HANDLE FcomHandle; // указатель на файл СОМ порта
	TIniFile *ini; // Объевление переменной для работы с ini файломи
	UnicodeString port;
	float div; // делитель количества импульсов
	unsigned int imp;      // состояние импульсного входа
	 float impCount; // счетчик импульсов
	unsigned int timeCount; // счетчик прохода цикла
	float waterAmount;
	float waterSupply;
	float drainage;

public:		// User declarations
	__fastcall TmainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TmainForm *mainForm;
//---------------------------------------------------------------------------
#endif
