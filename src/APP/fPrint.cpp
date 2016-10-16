// fPrint.exe       Print File
// Usage: filename = parameter; eg:
// fPrint.exe "D:\cpp\fPrint\Release\print.pcl"
//

#include "stdafx.h"
#include <windows.h>
#include <winspool.h>
#include <iostream>
#include <string.h>
#include <conio.h>
#include <shellapi.h>


using namespace std;

 
// PROTOTYPES:
void EnumeratePrinters(DWORD flags);
 

//====================================================================
int _get_printer (TCHAR *sprt, DWORD *lsprt){
//====================================================================
// get the default-printer into sprt

  int  irc = -1;

  TCHAR s1[256];
  DWORD l1 = 200;
  GetDefaultPrinter (s1, &l1);
  //_tprintf(_T("GetDefaultPrinter:|%s|\n"),s1);


	PRINTER_INFO_2* prninfo=NULL;
	DWORD needed, returned;
	DWORD flags = PRINTER_ENUM_LOCAL;

	//find required size for the buffer
	EnumPrinters (flags, NULL, 2, NULL, 0, &needed, &returned);

	//allocate array of PRINTER_INFO structures
	prninfo = (PRINTER_INFO_2*) GlobalAlloc(GPTR,needed);

	//call again
	if (!EnumPrinters(flags, NULL, 
		2, (LPBYTE) prninfo, needed, &needed, &returned))    {
		cout << "***** ERROR EnumPrinters " << GetLastError() << endl;

	}    else    {
		for (int i=0; i<returned; ++i)   {
            // compare pPrinterName
			//if(_tstrcmp(s1, prninfo[i].pPrinterName)) continue;
			if(_tcsstr(s1, prninfo[i].pPrinterName) == NULL) continue;
			// printer gefunden; pShareName kopieren.
			_tcscpy(sprt, prninfo[i].pShareName);
			irc = 0;

/*
			cout << "===============" << i << "===============" << endl;
			cout << "Status:" << prninfo[i].Status << endl;
			cout << "cJobs:" << prninfo[i].cJobs << endl;
			_tprintf(_T("pShareName:|%s|\n"),prninfo[i].pShareName);
			_tprintf(_T("pPrinterName:|%s|\n"),prninfo[i].pPrinterName);
			_tprintf(_T("pPortName:|%s|\n"),prninfo[i].pPortName);
			_tprintf(_T("pDriverName:|%s|\n"),prninfo[i].pDriverName);
			_tprintf(_T("pServerName:|%s|\n"),prninfo[i].pServerName);
			_tprintf(_T("pComment:|%s|\n"),prninfo[i].pComment);
			_tprintf(_T("pLocation:|%s|\n"),prninfo[i].pLocation);
*/
		}
	}

	GlobalFree(prninfo);
	return irc;
}
 


//====================================================================
int _tmain(int argc, _TCHAR* argv[]) {
//====================================================================

  //_tprintf(_T("%s\n"),argv[0]);  // display Arg[0]

/*
  if(argc < 2) {
	cout << "no filename to print .." << endl;
	exit (-1);
  }


  // test if file exists
  // Data structure for FindFirstFile
  WIN32_FIND_DATA findData;

  // Clear find structure
  ZeroMemory(&findData, sizeof(findData));

  // Search the file
  //HANDLE hFind = FindFirstFile (fileName, &findData);
  HANDLE hFind = FindFirstFile (argv[1], &findData);
  if (hFind == INVALID_HANDLE_VALUE) {
	cout << "file does not exist .." << endl;
	exit (-1);
  }
*/

  TCHAR prt[200];
  DWORD l1 = 200;
  int irc = _get_printer (prt, &l1);
  if(irc < 0) {
    cout << "could not get defaultprinter .." << endl;
    exit (-1);
  }
  // _tprintf(_T(" printer=|%s|\n"),s1);


  // prepare printername
  TCHAR host[200];

  l1 = 200;
  GetComputerName (host, &l1);
  //  _tprintf(_T(" compName=|%s|\n"),s1);

/*
  TCHAR cmd[1024];
  _tcscpy(cmd, _T("\\\\"));
  _tcscat(cmd, host);
  _tcscat(cmd, _T("\\\""));
  _tcscat(cmd, prt);
*/
  _tprintf(_T("\\\\%s\\\"%s\"\n"),host,prt);






/*
	ShellExecute (NULL,
		  TEXT("copy"),     // Oper
		  TEXT(""),         // File
		  TEXT("aaa bbb"),      // Param
		    NULL,           // Dir
			SW_HIDE);
*/



/*

    char test[] = "This is a test line to be printed.";
    
    LPHANDLE hPrinter;
    LPBYTE DocInfo;   
    LPVOID Buff = test;
    DWORD read;
    LPDWORD written;

    //OpenPrinter(_T("\\\\CADCAM-NRPAWKZ0\\LexmarkO"), hPrinter, NULL);
    OpenPrinter(NULL, hPrinter, NULL);
    StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo);
    StartPagePrinter(hPrinter);

	read = 12;
    WritePrinter(hPrinter, Buff, read, written); 

    EndPagePrinter(hPrinter);
    EndDocPrinter(hPrinter);
    ClosePrinter(hPrinter);





        // Allow the user to select a printer.
        PrintDialog pd  = new PrintDialog();
        pd.PrinterSettings = new PrinterSettings();
        //pd.ShowDialog(NULL);

        {
            // Print the file to the printer.
            //RawPrinterHelper.SendFileToPrinter(pd.PrinterSettings.PrinterName,  "X:\\Devel\\dev\\gCAD3D\\tmp\\print.eps");
            //RawPrinterHelper.SendFileToPrinter(NULL,  "X:\\Devel\\dev\\gCAD3D\\tmp\\print.eps");
        }
*/





    return 0;

}

