// vdc.cpp : Defines the entry point for the application.
//

#include "common.h"
#include "capture.h"
#include "cleanThread.h"

TCHAR szAppName[] = TEXT("TEST");		//��������
#define DESTOP_NAME  TEXT("MY_DESTOP")	//������Ҫ��������������
void createThunder();
void closeThunder();
TFindWindowThread* f;

#define REGHK 1
#define EXECHK 2

ATOM *HotKeyId; 
ATOM m_HotKeyId1;			//����ע��ctrl+shift+F6  ,�л���ԭ���Ļ����
ATOM m_HotKeyId2;			//����ע��ctrl+shift+F7	,��������л��������´���������,
ATOM m_HotKeyId3;			//������ȷ�˳�����,��Ϊ����û����Ӧ��������Ҫ�ͷ�һЩ��Դ
ATOM m_HotKeyId4;
ATOM m_HotKeyId5;

HDESK hDDesk;
 
HWINSTA  hWinStaThisProcess = GetProcessWindowStation( );			//������������,��ȡϵͳΪ�ý������õ�Ĭ�Ͻ�������վ
HDESK hDeskOriignInput = OpenInputDesktop(0,FALSE,GENERIC_ALL);		//������������,��ȡϵͳΪ���߳����õ�Ĭ�ϻ����
DWORD Pid;


	
void t(){
	HWND h;

	/*
	MsgVal(Pid);
	    h=GetWindowHwndByPID(Pid);
	    MsgVal( (long)h);
	if(h==0){
	    h=GetWindowHwndByPID2(Pid);
		MsgVal( (long)h);
	}
	if(h==0){
	    h=findWindow(TEXT("Ѹ��U���"));
		MsgVal( (long)h);
	}
	SendMessage(h, WM_SIZE, SIZE_MAXSHOW , 0); 
	SendMessage(h, WM_SYSCOMMAND, SC_MAXIMIZE, 0); // ���
	*/
	
	/*
	CaptureImage1(GetDesktopWindow(),"d:\\Myuse\\","CaptureImage1");
	CaptureImage2(GetDesktopWindow(),"d:\\Myuse\\CaptureImage2.bmp");
	CaptureImage3(GetDesktopWindow(),"d:\\Myuse\\","CaptureImage3");
	h=findHWND(4,Pid,TEXT("Ѹ��U���"));
	CaptureImage1(h,"d:\\Myuse\\","thunder1");
	CaptureImage2(h,"d:\\Myuse\\thunder2.bmp");
	CaptureImage3(h,"d:\\Myuse\\","thunder3");
	*/
	
	//h=findHWND(4,Pid,TEXT("Ѹ��U���"));
	//CaptureImage1(h,"d:\\Myuse\\","thunder1_main");

	f = new TFindWindowThread(Pid,hDDesk);  

		if( f=0 ){ 
			MessageBox(0,TEXT("TFindWindowThread Error"),0,0);
		  return ;
		}  
	
}

void  HotKey(int op,HWND hwnd,WPARAM wParam)
{

	/****************************************************************
				ע��3���ȼ�
	******************************************************************/
	if(op==REGHK){
		//LPCTSTR lps = "hehehehe";
		//��ȫ��ԭ�ӱ������һ���ַ����õ�idһ��,��һ���   CTRL +SHIFT+F6 �й�
		//m_HotKeyId1 = GlobalAddAtom(lps) - 0xc000;
	 	m_HotKeyId1 = GlobalAddAtom(TEXT("HotKey1")) - 0xc000;
		//ע��õ���id�� �ȼ�CTRL+SHIFT+F6 ��������
		//RegisterHotKey(hwnd,m_HotKeyId1,MOD_CONTROL | MOD_SHIFT,VK_F6);
	            	RegisterHotKey(hwnd,m_HotKeyId1,0,VK_F6);
	}else if(op==EXECHK){  if(wParam == m_HotKeyId1) 	{
				// ���ȼ�����ʱ�Ĵ���
				SwitchDesktop(hDeskOriignInput);	//ԭ����汻���ֳ���
				//MessageBox(0,TEXT("F6"),0,0);
	}}


	if(op==REGHK){
		//��ȫ��ԭ�ӱ������һ���ַ����õ�idһ��,��һ���   CTRL +SHIFT+F7 �й�
		m_HotKeyId2 = GlobalAddAtom(TEXT("HotKey2")) - 0xc000;
		//ע��õ���id�� �ȼ�CTRL+SHIFT+F7 ��������
		//RegisterHotKey(hwnd,m_HotKeyId2,MOD_CONTROL | MOD_SHIFT,VK_F7);
	            	RegisterHotKey(hwnd,m_HotKeyId2,0,VK_F7);
 	}else if(op==EXECHK){  if(wParam == m_HotKeyId2) 	{
 		SwitchDesktop(hDDesk);				//���Ǵ������������ֳ���
		//MessageBox(0,TEXT("F7"),0,0);
	}}


	HotKeyId = &m_HotKeyId4;
	if(op==REGHK){	
	//��ȫ��ԭ�ӱ������һ���ַ����õ�idһ��
	*HotKeyId = GlobalAddAtom(TEXT("HotKey4")) - 0xc000;
	//ע��õ���id�� �ȼ� ��������
		       RegisterHotKey(hwnd,*HotKeyId,0,VK_F9);
 	}else if(op==EXECHK&&wParam == *HotKeyId){ 	
		
 		createThunder();
		//MessageBox(0,TEXT("F7"),0,0);
	}
	
	
	if(op==REGHK){	
	//��ȫ��ԭ�ӱ������һ���ַ����õ�idһ��,��һ���   F7 �й�
	m_HotKeyId3 = GlobalAddAtom(TEXT("HotKey3")) - 0xc000;
	//ע��õ���id�� �ȼ�F7 ��������
	          RegisterHotKey(hwnd,m_HotKeyId3,0,VK_F8);
 	}else if(op==EXECHK&&wParam == m_HotKeyId3){ 	

 		MessageBox(0,TEXT("�����˳�"),0,0);
		PostQuitMessage(0);			//���Ǵ������������ֳ���
		delete f;
		//MessageBox(0,TEXT("F7"),0,0);
	}
	

	HotKeyId = &m_HotKeyId5;
	if(op==REGHK){	
	//��ȫ��ԭ�ӱ������һ���ַ����õ�idһ��
	*HotKeyId = GlobalAddAtom(TEXT("HotKey5")) - 0xc000;
	//ע��õ���id�� �ȼ� ��������
		       RegisterHotKey(hwnd,*HotKeyId,0,VK_F11);
 	}else if(op==EXECHK&&wParam == *HotKeyId){ 	

 		t();
		//MessageBox(0,TEXT("F7"),0,0);
	}
	/**********************************************************
	***********************************************************/
 
}

void regeditHotKey(HWND hwnd)
{
	HotKey(REGHK,hwnd,0);
}

void OnHotKey(WPARAM wParam,LPARAM lParam)
{
	static 
	LRESULT lRes = 0;
 	HWND hwnd=0;
	HotKey(EXECHK,hwnd,wParam);

	return ;
}

long createDesktop(HDESK *hDDesk,TCHAR* dN)
{
	 //�ڽ��� Ĭ�ϵĽ���ʽ����վ�д��� ����,
	*hDDesk = CreateDesktop(
		dN,
		//DESTOP_NAME,
		0,0,0,
		GENERIC_ALL,
		NULL
		);
	//�����뱾�߳�����ϵ������Ϊ ���Ǵ���������
	SetThreadDesktop(*hDDesk);

	return 1;
}

//ProcessInfo.hProcess�ǽ��̾��
//ProcessInfo.dwProcessId�ǽ���ID
bool createProcess(TCHAR* p,TCHAR* dN,DWORD* ppid){
	//��дSTARTUPINFO �ṹ����Ϣ,ע����ƽ���������̵Ĳ�ͬ,��������lpDesktopѡ�������,
	STARTUPINFO sti = {
		sizeof(sti)
	};
	sti.lpDesktop = dN;
 
	PROCESS_INFORMATION pi = {0};
	int r= CreateProcess(
		p,
		NULL,NULL,NULL,TRUE,0,NULL,NULL,
		&sti,&pi
		);
	if(r==1){
		 *ppid = pi.dwProcessId;
		 return true;
	}else{
		*ppid=0;
		return false;
	}
}

bool closeProcess(unsigned long pid){
if(pid==0) 
	return false;
HANDLE bExitCode = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE| PROCESS_ALL_ACCESS, FALSE
							, pid);

//WaitForSingleObject(bExitCode, INFINITE)
if (NULL != bExitCode)

{
BOOL bFlag = TerminateProcess(bExitCode, 0);
CloseHandle(bExitCode);
return true;
}
return false;
}





int mainProcess(HDESK *hDDesk)
{
 
	createDesktop(hDDesk,DESTOP_NAME);
	//createProcess(TEXT("C:\\Windows\\explorer.exe"),DESTOP_NAME);
	
	return 1;
}
void createThunder(){
	


	DWORD pid ;createProcess(TEXT("c:\\Program Files (x86)\\Thunder Network\\ThunderVIP\\Program\\Thunder.exe"),DESTOP_NAME,&pid);
	if(pid!=-1){
	/*
	long hWnd=findWindow();
	captureHWnd();
	*/
		Pid=pid;

		
	}
}


void closeThunder(){
//BOOL ret = CreateProcess(NULL,cCmdLine,NULL,NULL,FALSE,0,NULL,NULL,&startInfo,&procInfo);
	closeProcess(Pid);
}


typedef struct
{
    int    x;     // ���ھ��
    int   y;    // ����ID
}point;
int hWndOperate(long hWnd,int Operation,point Postion)
{
 

	return 1;
}

 void exitClean(HWND hwnd,HDESK *hDDesk){
	
	closeThunder();

	//�˳�,˳��������Դ
	UnregisterHotKey(hwnd,m_HotKeyId1);
	GlobalDeleteAtom(m_HotKeyId1);
 
	UnregisterHotKey(hwnd,m_HotKeyId2);
	GlobalDeleteAtom(m_HotKeyId2);
 
	UnregisterHotKey(hwnd,m_HotKeyId3);
	GlobalDeleteAtom(m_HotKeyId3);
 
 	CloseDesktop(*hDDesk);
	CloseHandle(*hDDesk);
}


LRESULT		CALLBACK	WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_HOTKEY:
		OnHotKey(wParam,lParam);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	HWND hwnd;
	MSG	 msg;
	WNDCLASS	wndclass;
 
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL,TEXT("����ע��ʧ��!"),NULL,0);
		return 0;
	}
	hwnd = CreateWindow(szAppName,szAppName,WS_OVERLAPPEDWINDOW,0,0,0,0,NULL,NULL,hInstance,NULL
		);
//-----------------------------------------------������windowsĬ�ϲ���----------------------------------
 
 mainProcess(&hDDesk);
 regeditHotKey(hwnd);
//-------------------------------------------------������Ϣѭ��-------------------------------------------
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
//-------------------------------------------------��Ϣѭ������-------------------------------------------



	exitClean(hwnd,&hDDesk);
	return msg.wParam;
}