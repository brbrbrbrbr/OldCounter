//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include <Math.hpp>
#include <math.h>


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pies"
#pragma resource "*.dfm"
TmainForm *mainForm;
//---------------------------------------------------------------------------
__fastcall TmainForm::TmainForm(TComponent* Owner): TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(GetCurrentDir() + "\\waterCounter.ini"); // �������� (����������� ���� ���� ����) �����.
	port = ini->ReadString("port", "port", "COM1");
	div = ini->ReadFloat("div", "div", 1);
	ini->Free();
	port = port.t_str();
	FcomHandle = CreateFile( // ��������� ��� ���� ��� ����
		port.t_str(),                  // ��� ����� - 'COM1', ���
		GENERIC_WRITE | GENERIC_READ, // ����� �������� GENERIC_READ - ������, GENERIC_WRITE - ������
		0,                    // ����� ���������� ��������. ����������: 0 - ������������� (������ ��� ������� �������� ����������������� ����� � WIN SDK, ������ ������ �� ��������).
		NULL,                  // ����� ������������. ����� ����� � Windows NT, Windows 9x ���������� ���.
		OPEN_EXISTING,        // ������ ��������. ��� ����� - OPEN_EXISTING - �������, ����� ���������� ������� ����������.
        FILE_ATTRIBUTE_NORMAL, // ����� ��������� �������� - FILE_FLAG_OVERLAPPED - ���������� ����� ��������. ��� ���� �������� ������ - ������, ��������� ������������� �������, ����������� ������ �� ��������� � ��������� ������ ���������.
        0                     // ������ �����, ��� ����������������� ����� - ������ 0.
	);
	if(FcomHandle == INVALID_HANDLE_VALUE)
	{
		ShowMessage("���� �� ������ :-("); 	// ��������� ������� �� ���� ��� ���
		//CloseHandle(FcomHandle);
		// ��������� ��� ����
		return;
	}
	SetupComm(&FcomHandle, 1600, 1600);

	DCB dcb;
	GetCommState(FcomHandle, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize =  8;
	dcb.Parity = EVENPARITY;  //parity
	dcb.StopBits = 0;                         // 0,1,2 = 1, 1.5, 2
	dcb.fBinary = true;                                    //�������� �������� ����� ������
	dcb.fOutxCtsFlow = true;                              //��������� ����� �������� �� �������� CTS
	dcb.fOutxDsrFlow = true;                              //��������� ����� �������� �� �������� DSR
	dcb.fDtrControl = DTR_CONTROL_ENABLE;                 //��������� ������������� ����� DTR
	dcb.fDsrSensitivity = FALSE;                           //��������� ��������������� �������� � ��������� ����� DSR
	dcb.fNull = FALSE;                                     //��������� ���� ������� ������
	dcb.fRtsControl = RTS_CONTROL_ENABLE;                 //��������� ������������� ����� RTS
	dcb.fAbortOnError = FALSE;
	dcb.DCBlength = sizeof(DCB);

	if(!SetCommState(FcomHandle, &dcb))
	{
		ShowMessage("���� �� �������� :-(");
		CloseHandle(FcomHandle);
		// ��������� ��� ����
		return;
	}

	EscapeCommFunction(&FcomHandle, SETRTS);
	EscapeCommFunction(&FcomHandle, SETDTR);
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::Button1Click(TObject *Sender)
{
	Button1->Visible = false;
	impCount = 0;
	waterAmount = 0;
	waterSupply = 0;
	drainage = 0;
	while(1)
   {
	   DWORD dwModemStat;
	   GetCommModemStatus(FcomHandle, &dwModemStat);

	   if((dwModemStat && MS_CTS_ON) && (imp == 0))
	   {
		  impCount++;
		  timeCount = 0;
		  Application->ProcessMessages();

	   }





	   imp = (unsigned int)(dwModemStat&MS_CTS_ON);


	   waterAmount =  impCount*1.5;
	   waterSupply =  waterAmount*0.000001*748;
	   drainage =  waterAmount*0.000001*595;



	   Label1->Caption = IntToStr((int)impCount);
	   Label2->Caption = FloatToStr(waterAmount)+ " ml";
	   Label5->Caption = String(FormatFloat(000.0, waterSupply))+ " ���.";
	   Label6->Caption = String(FormatFloat(000.0, drainage))+ " ���.";
	   Label12->Caption = String(FormatFloat(000.0, drainage + waterSupply))+ " ���.";









	   pie->Angles->StartAngle = 360 - ((int)impCount / div);

	   if(pie->Angles->StartAngle < 120)
			pie->Brush->Color = clRed;
	   else if(pie->Angles->StartAngle < 120 * 2)
			pie->Brush->Color = clYellow;

	   if(timeCount++ > 900000) // ���������� ������� ��������� ��� ������������ �������
	   {
			impCount = 0;
			timeCount = 0;
			pie->Brush->Color = clLime;
			Application->ProcessMessages();
	   }
	   if(timeCount % 1024)
			Application->ProcessMessages();
   }
  /*
   MS_CTS_ON = 0x10;
   MS_DSR_ON = 0x20;
   MS_RING_ON = 0x40;
   MS_RLSD_ON  = 0x80;
   */
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::FormResize(TObject *Sender)
{
	//pie->Width = mainForm->Width - 5;
	//pie->Height = mainForm->Height - 50;
}
//---------------------------------------------------------------------------
