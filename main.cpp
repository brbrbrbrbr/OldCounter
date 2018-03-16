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
	ini = new TIniFile(GetCurrentDir() + "\\waterCounter.ini"); // Создание (подключение если файл есть) файла.
	port = ini->ReadString("port", "port", "COM1");
	div = ini->ReadFloat("div", "div", 1);
	ini->Free();
	port = port.t_str();
	FcomHandle = CreateFile( // открываем СОМ порт как файл
		port.t_str(),                  // имя порта - 'COM1', итд
		GENERIC_WRITE | GENERIC_READ, // режим открытия GENERIC_READ - чтение, GENERIC_WRITE - запись
		0,                    // режим разделения ресуртса. Примечание: 0 - неразделяемый (именно так описано открытие последовательного порта в WIN SDK, другие режимы не проверял).
		NULL,                  // режим безопасности. Имеет смысл в Windows NT, Windows 9x игнорирует его.
		OPEN_EXISTING,        // способ открытия. Для порта - OPEN_EXISTING - открыть, когда устройство реально существует.
        FILE_ATTRIBUTE_NORMAL, // режим наложения операций - FILE_FLAG_OVERLAPPED - разрешение таких операций. При этом операции чтения - записи, требующие значительного времени, выполняются фоново по отношению к основному потоку программы.
        0                     // шаблон файла, для последовательного порта - всегда 0.
	);
	if(FcomHandle == INVALID_HANDLE_VALUE)
	{
		ShowMessage("порт не открыт :-("); 	// проверяем открыли мы порт или нет
		//CloseHandle(FcomHandle);
		// закрываем СОМ порт
		return;
	}
	SetupComm(&FcomHandle, 1600, 1600);

	DCB dcb;
	GetCommState(FcomHandle, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize =  8;
	dcb.Parity = EVENPARITY;  //parity
	dcb.StopBits = 0;                         // 0,1,2 = 1, 1.5, 2
	dcb.fBinary = true;                                    //включаем двоичный режим обмена
	dcb.fOutxCtsFlow = true;                              //выключаем режим слежения за сигналом CTS
	dcb.fOutxDsrFlow = true;                              //выключаем режим слежения за сигналом DSR
	dcb.fDtrControl = DTR_CONTROL_ENABLE;                 //отключаем использование линии DTR
	dcb.fDsrSensitivity = FALSE;                           //отключаем восприимчивость драйвера к состоянию линии DSR
	dcb.fNull = FALSE;                                     //разрешить приём нулевых байтов
	dcb.fRtsControl = RTS_CONTROL_ENABLE;                 //отключаем использование линии RTS
	dcb.fAbortOnError = FALSE;
	dcb.DCBlength = sizeof(DCB);

	if(!SetCommState(FcomHandle, &dcb))
	{
		ShowMessage("порт не настроен :-(");
		CloseHandle(FcomHandle);
		// закрываем СОМ порт
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
	   Label5->Caption = String(FormatFloat(000.0, waterSupply))+ " коп.";
	   Label6->Caption = String(FormatFloat(000.0, drainage))+ " коп.";
	   Label12->Caption = String(FormatFloat(000.0, drainage + waterSupply))+ " коп.";









	   pie->Angles->StartAngle = 360 - ((int)impCount / div);

	   if(pie->Angles->StartAngle < 120)
			pie->Brush->Color = clRed;
	   else if(pie->Angles->StartAngle < 120 * 2)
			pie->Brush->Color = clYellow;

	   if(timeCount++ > 900000) // сбрасываем счетчик импульсов при переполнении таймера
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
