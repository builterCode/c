// vdc.cpp : Defines the entry point for the application.
//

#include "common.h"
#include "capture.h"
#include "cleanThread.h"

TCHAR szAppName[] = TEXT("TEST");		//窗口类名
#define DESTOP_NAME  TEXT("MY_DESTOP")	//定义我要创建的桌面名称
void createThunder();
void closeThunder();
TFindWindowThread* f;

#define REGHK 1
#define EXECHK 2

ATOM *HotKeyId; 
ATOM m_HotKeyId1;			//用于注册ctrl+shift+F6  ,切换到原来的活动桌面
ATOM m_HotKeyId2;			//用于注册ctrl+shift+F7	,将活动桌面切换到我们新创建的桌面,
ATOM m_HotKeyId3;			//用于正确退出程序,因为程序没有相应界面且需要释放一些资源
ATOM m_HotKeyId4;
ATOM m_HotKeyId5;

HDESK hDDesk;
 
HWINSTA  hWinStaThisProcess = GetProcessWindowStation( );			//程序运行起来,获取系统为该进程设置的默认交互工作站
HDESK hDeskOriignInput = OpenInputDesktop(0,FALSE,GENERIC_ALL);		//程序运行起来,获取系统为该线程设置的默认活动桌面
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
	    h=findWindow(TEXT("迅雷U享版"));
		MsgVal( (long)h);
	}
	SendMessage(h, WM_SIZE, SIZE_MAXSHOW , 0); 
	SendMessage(h, WM_SYSCOMMAND, SC_MAXIMIZE, 0); // 最大化
	*/
	
	/*
	CaptureImage1(GetDesktopWindow(),"d:\\Myuse\\","CaptureImage1");
	CaptureImage2(GetDesktopWindow(),"d:\\Myuse\\CaptureImage2.bmp");
	CaptureImage3(GetDesktopWindow(),"d:\\Myuse\\","CaptureImage3");
	h=findHWND(4,Pid,TEXT("迅雷U享版"));
	CaptureImage1(h,"d:\\Myuse\\","thunder1");
	CaptureImage2(h,"d:\\Myuse\\thunder2.bmp");
	CaptureImage3(h,"d:\\Myuse\\","thunder3");
	*/
	
	//h=findHWND(4,Pid,TEXT("迅雷U享版"));
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
				注册3个热键
	******************************************************************/
	if(op==REGHK){
		//LPCTSTR lps = "hehehehe";
		//在全局原子表中添加一项字符串得到id一个,这一项跟   CTRL +SHIFT+F6 有关
		//m_HotKeyId1 = GlobalAddAtom(lps) - 0xc000;
	 	m_HotKeyId1 = GlobalAddAtom(TEXT("HotKey1")) - 0xc000;
		//注册得到的id与 热键CTRL+SHIFT+F6 关联起来
		//RegisterHotKey(hwnd,m_HotKeyId1,MOD_CONTROL | MOD_SHIFT,VK_F6);
	            	RegisterHotKey(hwnd,m_HotKeyId1,0,VK_F6);
	}else if(op==EXECHK){  if(wParam == m_HotKeyId1) 	{
				// 该热键按下时的处理
				SwitchDesktop(hDeskOriignInput);	//原活动桌面被显现出来
				//MessageBox(0,TEXT("F6"),0,0);
	}}


	if(op==REGHK){
		//在全局原子表中添加一项字符串得到id一个,这一项跟   CTRL +SHIFT+F7 有关
		m_HotKeyId2 = GlobalAddAtom(TEXT("HotKey2")) - 0xc000;
		//注册得到的id与 热键CTRL+SHIFT+F7 关联起来
		//RegisterHotKey(hwnd,m_HotKeyId2,MOD_CONTROL | MOD_SHIFT,VK_F7);
	            	RegisterHotKey(hwnd,m_HotKeyId2,0,VK_F7);
 	}else if(op==EXECHK){  if(wParam == m_HotKeyId2) 	{
 		SwitchDesktop(hDDesk);				//我们创建的桌面显现出来
		//MessageBox(0,TEXT("F7"),0,0);
	}}


	HotKeyId = &m_HotKeyId4;
	if(op==REGHK){	
	//在全局原子表中添加一项字符串得到id一个
	*HotKeyId = GlobalAddAtom(TEXT("HotKey4")) - 0xc000;
	//注册得到的id与 热键 关联起来
		       RegisterHotKey(hwnd,*HotKeyId,0,VK_F9);
 	}else if(op==EXECHK&&wParam == *HotKeyId){ 	
		
 		createThunder();
		//MessageBox(0,TEXT("F7"),0,0);
	}
	
	
	if(op==REGHK){	
	//在全局原子表中添加一项字符串得到id一个,这一项跟   F7 有关
	m_HotKeyId3 = GlobalAddAtom(TEXT("HotKey3")) - 0xc000;
	//注册得到的id与 热键F7 关联起来
	          RegisterHotKey(hwnd,m_HotKeyId3,0,VK_F8);
 	}else if(op==EXECHK&&wParam == m_HotKeyId3){ 	

 		MessageBox(0,TEXT("程序退出"),0,0);
		PostQuitMessage(0);			//我们创建的桌面显现出来
		delete f;
		//MessageBox(0,TEXT("F7"),0,0);
	}
	

	HotKeyId = &m_HotKeyId5;
	if(op==REGHK){	
	//在全局原子表中添加一项字符串得到id一个
	*HotKeyId = GlobalAddAtom(TEXT("HotKey5")) - 0xc000;
	//注册得到的id与 热键 关联起来
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
	 //在进程 默认的交互式工作站中创建 桌面,
	*hDDesk = CreateDesktop(
		dN,
		//DESTOP_NAME,
		0,0,0,
		GENERIC_ALL,
		NULL
		);
	//设置与本线程相联系的桌面为 我们创建的桌面
	SetThreadDesktop(*hDDesk);

	return 1;
}

//ProcessInfo.hProcess是进程句柄
//ProcessInfo.dwProcessId是进程ID
bool createProcess(TCHAR* p,TCHAR* dN,DWORD* ppid){
	//填写STARTUPINFO 结构体信息,注意与平常创建进程的不同,这里有了lpDesktop选项的设置,
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
    int    x;     // 窗口句柄
    int   y;    // 进程ID
}point;
int hWndOperate(long hWnd,int Operation,point Postion)
{
 

	return 1;
}

 void exitClean(HWND hwnd,HDESK *hDDesk){
	
	closeThunder();

	//退出,顺便清理资源
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
		MessageBox(NULL,TEXT("窗口注册失败!"),NULL,0);
		return 0;
	}
	hwnd = CreateWindow(szAppName,szAppName,WS_OVERLAPPEDWINDOW,0,0,0,0,NULL,NULL,hInstance,NULL
		);
//-----------------------------------------------以上是windows默认部分----------------------------------
 
 mainProcess(&hDDesk);
 regeditHotKey(hwnd);
//-------------------------------------------------窗口消息循环-------------------------------------------
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
//-------------------------------------------------消息循环结束-------------------------------------------



	exitClean(hwnd,&hDDesk);
	return msg.wParam;
}