#include "cleanThread.h"
#include "capture.h"

typedef struct SParam  
{  
    long pid;  
    HDESK FDesktop;  
    unsigned short sessionID;  
}uParam,*sParam;  

SParam sparam;
void TFindWindowThread::Close(){
	CloseHandle(hThread); 
}

static DWORD WINAPI   Execute(PVOID pParam){
	//int i; 

	SParam *sparam;  
    sparam = (sParam)pParam;  
	
	SParam * p=sparam;
    long	Pid = p->pid;
	HDESK FDesktop = p->FDesktop;
	
          /*Run为自己写的一个方法,Run(int i,unsigned short chnlID,unsigned short sessionID)*/  
          //Run(sparam->No,sparam->chnlID,sparam->sessionID);  



  if(!SetThreadDesktop(FDesktop)){
  		return -1;
  }
  //Sleep(500);  

  MsgVal(Pid);
	HWND h;
	//h=0;
	 int	c=3+1;
	h=findHWND(c,Pid,TEXT("迅雷U享版"));
	CaptureImage1(h,"d:\\Myuse\\","thunder1_thread");

	SendMessage(h, WM_SIZE, SIZE_MAXSHOW , 0); 
	SendMessage(h, WM_SYSCOMMAND, SC_MAXIMIZE, 0); // 最大化
    
  

 //CaptureImage2(h,"d:\\Myuse\\thunder1.bmp");
 //CaptureImage2(GetDesktopWindow(),"d:\\Myuse\\thunder1_thread.bmp");

}



TFindWindowThread::TFindWindowThread(long pid,const HDESK  ADesktop)
{
  FDesktop=ADesktop;
  Pid=pid;

    
    SParam *p;  
  
    sparam.pid = Pid;  
    sparam.FDesktop = FDesktop;  
    sparam.sessionID = 1;  
    p = &sparam;  

  hThread = CreateThread(NULL,0,Execute,p,0,&threadID); // 创建线程
}
