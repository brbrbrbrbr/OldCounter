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
#include <ExtCtrls.hpp> // ����������� ����������
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
	HANDLE FcomHandle; // ��������� �� ���� ��� �����
	TIniFile *ini; // ���������� ���������� ��� ������ � ini �������
	UnicodeString port;
	float div; // �������� ���������� ���������
	unsigned int imp;      // ��������� ����������� �����
	 float impCount; // ������� ���������
	unsigned int timeCount; // ������� ������� �����
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
