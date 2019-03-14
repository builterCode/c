#include "common.h"


HWND GetWindowHwndByPID(DWORD dwProcessID);
HWND GetWindowHwndByPID2(DWORD dwProcessID);
HWND findWindow(TCHAR* windowName);
HWND findHWND(int way,unsigned long Pid,TCHAR* text);

int CaptureImage1(HWND hwnd,const char *dirPath, const char *filename);
int CaptureImage2(HWND hwnd, CHAR *FilePath);
int CaptureImage3(HWND hwnd, CHAR *dirPath, CHAR *filename);