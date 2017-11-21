// TelEscrowBJ.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TelEscrowBJ.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define WM_MYMESSAGE	(WM_USER+100)
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTelEscrowBJApp

BEGIN_MESSAGE_MAP(CTelEscrowBJApp, CWinApp)
	//{{AFX_MSG_MAP(CTelEscrowBJApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelEscrowBJApp construction
#pragma data_seg(".SHARDAT")
static HHOOK hhkHook=NULL;                            //���平�Ӿ��
static HINSTANCE hInstance =NULL;                  //����ʵ��
char telPhone[12] = {0};
int telPos = 0;
CString log = "";
#define MAX_LENGTH 500
#define CONFIG_PSTR _T("IncrementSystem.ini")
static CString title;
#pragma data_seg()

extern "C"
BOOL WINAPI DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
	hInstance=(HINSTANCE)hModule;
	memset( telPhone, 1, sizeof(telPhone) );
    return TRUE;
}

//���Ǵ��������Ϣ����Ҫ�����������н��н�ֹ����

LRESULT CALLBACK HookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if (nCode < 0){
		return CallNextHookEx(hhkHook,nCode,wParam,lParam);
	}
	if (nCode != HC_ACTION){
		return CallNextHookEx(hhkHook,nCode,wParam,lParam);
	}

	// ��ȡִ�г���Ŀ¼
	CString	sPath;
	GetModuleFileName(hInstance,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer    ();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 

	// ��ȡ�����ļ��� ִ���ļ���title
	CString configFile;
	configFile.Format("%s\\%s", sPath, CONFIG_PSTR);
	GetPrivateProfileString("System","Title","", title.GetBuffer(MAX_LENGTH),MAX_LENGTH,configFile);
	
	HWND h_Wnd = FindWindow(NULL, title);
	::PostMessage(h_Wnd, WM_MYMESSAGE, wParam, lParam);

	return CallNextHookEx(hhkHook,nCode,wParam,lParam);
    //return 1;   //û��return CallNextHookEx(hhkHook,nCode,wParam,lParam)�򲻻����Ϣ//������ȥ���������ǵļ��̾Ͳ���������
}

// This is an example of an exported variable 

//����������������������

BOOL EnableKeyboardCapture()
{
	if(!(hhkHook=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)HookProc,hInstance,0)))
		return FALSE;
	return TRUE;
}

//���������������������

BOOL DisableKeyboardCapture()
{
	return UnhookWindowsHookEx(hhkHook);
}


CTelEscrowBJApp::CTelEscrowBJApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTelEscrowBJApp object

CTelEscrowBJApp theApp;
