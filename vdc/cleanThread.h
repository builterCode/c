#include "common.h"


class TFindWindowThread{

private: 
   HDESK  FDesktop;  
   HANDLE hThread; 
   long Pid;
  protected :
    
  public:
    TFindWindowThread(long pid,HDESK ADesktop);
    DWORD threadID;
	void Close();
	//static DWORD WINAPI Execute(PVOID pParam);
};
  