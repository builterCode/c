#include "capture.h"

///< 枚举窗口参数
typedef struct
{
    HWND    hwndWindow;     // 窗口句柄
    DWORD   dwProcessID;    // 进程ID
}EnumWindowsArg;
///< 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    EnumWindowsArg *pArg = (EnumWindowsArg *)lParam;
    DWORD  dwProcessID = 0;
    // 通过窗口句柄取得进程ID
    GetWindowThreadProcessId(hwnd, &dwProcessID);
    if (dwProcessID == pArg->dwProcessID)
    {
        pArg->hwndWindow = hwnd;
        // 找到了返回FALSE
        return FALSE;
    }
    // 没找到，继续找，返回TRUE
    return TRUE;
}
///< 通过进程ID获取窗口句柄
HWND GetWindowHwndByPID(DWORD dwProcessID)
{
    HWND hwndRet = NULL;
    EnumWindowsArg ewa;
    ewa.dwProcessID = dwProcessID;
    ewa.hwndWindow = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
    if (ewa.hwndWindow)
    {
        hwndRet = ewa.hwndWindow;
    }
    return hwndRet;
}

///< 通过进程ID获取窗口句柄
HWND GetWindowHwndByPID2(DWORD dwProcessID)
{
    DWORD dwPID = 0;
    HWND hwndRet = NULL;
    // 取得第一个窗口句柄
    HWND hwndWindow = ::GetTopWindow(0);
    while (hwndWindow)
    {
        dwPID = 0;
        // 通过窗口句柄取得进程ID
        DWORD dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPID);
        if (dwTheardID != 0)
        {
            // 判断和参数传入的进程ID是否相等
            if (dwPID == dwProcessID)
            {
                // 进程ID相等，则记录窗口句柄
                hwndRet = hwndWindow;
                break;
            }
        }
        // 取得下一个窗口句柄
        hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT);
    }
    // 上面取得的窗口，不一定是最上层的窗口，需要通过GetParent获取最顶层窗口
    HWND hwndWindowParent = NULL;
    // 循环查找父窗口，以便保证返回的句柄是最顶层的窗口句柄
    while (hwndRet != NULL)
    {
        hwndWindowParent = ::GetParent(hwndRet);
        if (hwndWindowParent == NULL)
        {
            break;
        }
        hwndRet = hwndWindowParent;
    }
    // 返回窗口句柄
    return hwndRet;
}

/*-------------------------------------------------------
FindWindow()内需一字不漏的填入完整的窗口标题名称，否则会找不到目标窗口；所以我们进一步利用 GetWindow()函式，搜寻全部可见窗口，再逐一比对其窗口标题名称，这样就可做到所谓「概略搜寻」，以下的程序范例会将桌面上所有窗口标题名称含有”记事本”字样的窗口通通关闭 
*/
#include<stdio.h>
#include<iostream>
#include<string>
using   namespace   std;
#include<cstring>

int findString(string str1,string str2){
    char a[] = "abcddabc";
    char b[] = "dda";
    int j;
    //方法一
    int i = str1.find(str2);  //返回即子字符串索引3
    //方法二
    char *rel = strstr(a, b); //首次出现地址，strstr保存的是ddabc
    if (rel != NULL)
        j = rel -a;         //根据返回子字符串匹配结果输出索引位

    size_t found = str1.find(str2);
    if (found!=std::string::npos){
    std::cout << "found at: " << found << '\n';
         return found;
    }
    else{
        return -1;
    }

}

HWND findWindow(TCHAR* windowName)
{
      //HANDLE hCurrentWindow = GetWindow(Handle,GW_HWNDFIRST);
      HWND hCurrentWindow = GetDesktopWindow();
      char buff[255];
      string Text;

      while (hCurrentWindow!=0)
      {
           if ( ( GetWindowText(hCurrentWindow,buff,255)>0) &&
           IsWindowVisible(hCurrentWindow) )
           {
                //ShowMessage(buff);
                Text=buff;
                if (findString(Text,windowName)!=-1)
               {
                       //do
                       SendMessage(hCurrentWindow, WM_SIZE, SIZE_MAXSHOW , 0); 
						SendMessage(hCurrentWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0); // 最大化
						return hCurrentWindow;
                       // 最小化指定的程序
                        /*
                        CloseWindow(hCurrentWindow); (勿以为这是「结束」指定程序的用法)
                        ShowWindow(hCurrentWindow, SW_MINIMIZE);
                        SendMessage(hCurrentWindow, WM_SIZE, SIZE_MINIMIZED, 0);
                       PostMessage(hCurrentWindow, WM_CLOSE, 0, 0);
                       */
              }
          }
          hCurrentWindow = GetWindow(hCurrentWindow,GW_HWNDNEXT);
     }
		return 0;
}

HWND findHWND(int way,unsigned long Pid,TCHAR* text){
	if(way==1||way==2){
		if(Pid==0) return 0;}
	else if(way==3||way==4){
		if(text==NULL) return 0;}

	HWND h;
	h=0;
	int c=way;
	switch(c){

		case 1:
	    h=GetWindowHwndByPID(Pid);
		MsgVal( (long)h);
		if(h!=0){break;}
	
		case 2:
	    h=GetWindowHwndByPID2(Pid);
		MsgVal( (long)h);
		if(h!=0){break;}
	
		case 3:
		if(text!=NULL){
	    h=findWindow(text);
		MsgVal( (long)h);
		if(h!=0){break;}
			}
		case 4:
		if(text!=NULL){
	    h=FindWindow(0,text);
		MsgVal( (long)h);
		if(h!=0){break;}
		}
		default:
			MessageBox(0,TEXT("no found h=0"),0,0);

   }
	return h;
}

/**
 * GDI 截屏函数
 * 参数 hwnd   要截屏的窗口句柄
 * 参数 dirPath    截图存放目录
 * 参数 filename 截图名称
 */
int CaptureImage1(HWND hwnd, const char *dirPath, const char *filename)
{
    HANDLE hDIB;
    HANDLE hFile;
    DWORD dwBmpSize;
    DWORD dwSizeofDIB;
    DWORD dwBytesWritten;
    CHAR FilePath[MAX_PATH];
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;
    CHAR *lpbitmap;
    INT width = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽
    INT height = GetSystemMetrics(SM_CYSCREEN);  // 屏幕高
    HDC hdcScreen = GetDC(NULL); // 全屏幕DC
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // 创建兼容内存DC
 
	RECT rcClient;
	// 获取客户端区域用于计算大小
    //GetClientRect(hwnd,&rcClient); //.left and .top equals 0
	::GetWindowRect(hwnd,&rcClient);
    int pic_width=rcClient.right - rcClient.left; 
	int pic_height=rcClient.bottom - rcClient.top;
	 //rcClient.left=rcClient.top=300;
	MsgStr("after rcClient");

    if (!hdcMemDC)
    {
        DeleteObject(hbmScreen);
        DeleteObject(hdcMemDC);
        ReleaseDC(NULL, hdcScreen);
		MsgStr("exit in hdcMemDC");
        return 0;
    }

    // 通过窗口DC 创建一个兼容位图
    //hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
	  hbmScreen = CreateCompatibleBitmap(hdcScreen, pic_width, pic_height);
    if (!hbmScreen)
    {
        DeleteObject(hbmScreen);
        DeleteObject(hdcMemDC);
        ReleaseDC(NULL, hdcScreen);
		MsgStr("exit in hbmScreen");
        return 0;
    }
    // 将位图块传送到我们兼容的内存DC中
    SelectObject(hdcMemDC, hbmScreen);
    if (!BitBlt(
        hdcMemDC,    // 目的DC
        0,0,        // 目的DC的 x,y 坐标
        pic_width,pic_height, // 目的 DC 的宽高
        hdcScreen,   // 来源DC
        rcClient.left, rcClient.top,        // 来源DC的 x,y 坐标
        SRCCOPY))    // 粘贴方式
    {
        DeleteObject(hbmScreen);
        DeleteObject(hdcMemDC);
        ReleaseDC(NULL, hdcScreen);
		MsgStr("exit in BitBlt");
        return 0;
    }

    // 获取位图信息并存放在 bmpScreen 中
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // 在 32-bit Windows 系统上, GlobalAlloc 和 LocalAlloc 是由 HeapAlloc 封装来的
    // handle 指向进程默认的堆. 所以开销比 HeapAlloc 要大
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char *)GlobalLock(hDIB);

    // 获取兼容位图的位并且拷贝结果到一个 lpbitmap 中.
    GetDIBits(
        hdcScreen,  // 设备环境句柄
        hbmScreen,  // 位图句柄
        0,          // 指定检索的第一个扫描线
        (UINT)bmpScreen.bmHeight, // 指定检索的扫描线数
        lpbitmap,   // 指向用来检索位图数据的缓冲区的指针
        (BITMAPINFO *)&bi, // 该结构体保存位图的数据格式
        DIB_RGB_COLORS // 颜色表由红、绿、蓝（RGB）三个直接值构成
    );


    wsprintf(FilePath, "%s\\%s.jpg", dirPath, filename);

    // 创建一个文件来保存文件截图
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // 将 图片头(headers)的大小, 加上位图的大小来获得整个文件的大小
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // 设置 Offset 偏移至位图的位(bitmap bits)实际开始的地方
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // 文件大小
    bmfHeader.bfSize = dwSizeofDIB;

    // 位图的 bfType 必须是字符串 "BM"
    bmfHeader.bfType = 0x4D42; //BM

    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    // 解锁堆内存并释放
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    // 关闭文件句柄
    CloseHandle(hFile);

    // 清理资源
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);

	MsgStr("pic saved");
    return 0;
}


/**
 * GDI 截取指定窗口
 * 
 * 参数 hwnd   要截屏的窗口句柄
 * 参数 FilePath    截图存放目录
 *
 */
/* 
char filePath[500];
FileSelectPopup ("c:\\Users\\Administrator\\Desktop", "*.bmp", "", "", VAL_SAVE_BUTTON, 0, 0, 1, 1, filePath);


*/
int CaptureImage2(HWND hwnd, CHAR *FilePath)
{
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    RECT rcClient;
    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;
    DWORD dwBmpSize;
    HANDLE hDIB;
    CHAR *lpbitmap;
    HANDLE hFile;
    DWORD dwSizeofDIB;
    DWORD dwBytesWritten;
	int width=0,height=0;

    hdcWindow = GetWindowDC(hwnd); // 截图目标窗口DC
//    hdcWindow = GetDC(hwnd); // 截图DC
 
    // 创建兼容内存DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 
 
    if(!hdcMemDC)
    {
        MessageBox(0,"CreateCompatibleDC has failed",0,0);
        goto done;
    }
 
    // 获取客户端区域用于计算大小
    GetClientRect(hwnd, &rcClient);
 
    // 设置延展模式
    SetStretchBltMode(hdcWindow, HALFTONE);
 
    // 来源 DC 是整个屏幕而目标 DC 是当前的窗口 (HWND)
/*    if(!StretchBlt(hdcWindow,                      //-----------注释原因：不需要压缩位图，又不截取全屏
        0,0, 
        rcClient.right, rcClient.bottom, 
        hdcScreen,                            
        0,0,
        GetSystemMetrics (SM_CXSCREEN),
        GetSystemMetrics (SM_CYSCREEN),
        SRCCOPY))
    {
        MessagePopup("","StretchBlt has failed");
        goto done;
    }
*/ 
	 width=rcClient.right - rcClient.left; 
	 height=rcClient.bottom - rcClient.top;
	 //width=400;height=300;	

    // 通过窗口DC 创建一个兼容位图
    hbmScreen = CreateCompatibleBitmap(
        hdcWindow,
        width, height//--位图宽高
        );
 
    if(!hbmScreen)
    {
        MessageBox(0,"CreateCompatibleBitmap Failed",0,0);
        goto done;
    }
 
    // 将位图块传送到我们兼容的内存DC中
    SelectObject(hdcMemDC,hbmScreen);
    //---------------------------注释原因：截取软件部分界面，需要设定来源DC的坐标以及目标DC的宽高，保留原代码以观赏  
    if(!BitBlt(
        hdcMemDC,   // 目的DC
        0,0,        // 目的DC的 x,y 坐标
        width,height, // 目的 DC 的宽高
        hdcWindow,  // 来源DC
        0,0,        // 来源DC的 宽高
        SRCCOPY))   // 粘贴方式
    {
        MessageBox(0,"BitBlt has failed",0,0);
        goto done;
    }
      /*
    if(!BitBlt(
        hdcMemDC,   // 目的DC
        0,0,        // 目的DC的 x,y 坐标
        400, 300, // 目的 DC 的宽高
        hdcWindow,  // 来源DC
        50,50+26,        // 来源DC的宽高
        SRCCOPY))   // 粘贴方式
    {
        MessageBox(0,"BitBlt has failed",0,0);
        goto done;
    }
 */
    // 获取位图信息并存放在 bmpScreen 中
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);
 
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;
 
    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
 
    // 在 32-bit Windows 系统上, GlobalAlloc 和 LocalAlloc 是由 HeapAlloc 封装来的
    // handle 指向进程默认的堆. 所以开销比 HeapAlloc 要大
    hDIB = GlobalAlloc(GHND,dwBmpSize); 
    lpbitmap = (char *)GlobalLock(hDIB);    
 
    // 获取兼容位图的位并且拷贝结果到一个 lpbitmap 中.
    GetDIBits(
        hdcWindow,  // 设备环境句柄
        hbmScreen,  // 位图句柄
        0,          // 指定检索的第一个扫描线
        (UINT)bmpScreen.bmHeight, // 指定检索的扫描线数
        lpbitmap,   // 指向用来检索位图数据的缓冲区的指针
        (BITMAPINFO *)&bi, // 该结构体保存位图的数据格式
        DIB_RGB_COLORS // 颜色表由红、绿、蓝（RGB）三个直接值构成
        );
 
 
 
    // 创建一个文件来保存文件截图
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
 
    // 将 图片头(headers)的大小, 加上位图的大小来获得整个文件的大小
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    // 设置 Offset 偏移至位图的位(bitmap bits)实际开始的地方
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
 
    // 文件大小
    bmfHeader.bfSize = dwSizeofDIB; 
 
    // 位图的 bfType 必须是字符串 "BM"
    bmfHeader.bfType = 0x4D42; //BM   
 
    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
 
    // 解锁堆内存并释放
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);
 
    // 关闭文件句柄
    CloseHandle(hFile);
 
    // 清理资源
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(hwnd,hdcWindow);
 
    return 0;
}


/**
 * GDI 截取指定窗口
 * 
 * 参数 hwnd   要截屏的窗口句柄
 * 参数 dirPath    截图存放目录
 * 参数 filename 截图名称
 */
int CaptureImage3(HWND hwnd, CHAR *dirPath, CHAR *filename)
{
    /*
	HDC mdc;
    HBITMAP hbmp;
	*/
    CHAR FilePath[MAX_PATH];
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    RECT rcClient;
    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;
    DWORD dwBmpSize;
    HANDLE hDIB;
    CHAR *lpbitmap;
    HANDLE hFile;
    DWORD dwSizeofDIB;
    DWORD dwBytesWritten;
 
    hdcScreen = GetDC(NULL); // 全屏幕DC
    hdcWindow = GetDC(hwnd); // 截图目标窗口DC
 
    // 创建兼容内存DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 
 
    if(!hdcMemDC)
    {
        MessageBox(0,TEXT("CreateCompatibleDC has failed"),0,0);
        goto done;
    }
 
    // 获取客户端区域用于计算大小
    GetClientRect(hwnd, &rcClient);
 
    // 设置延展模式
    SetStretchBltMode(hdcWindow, HALFTONE);
 
    // 来源 DC 是整个屏幕而目标 DC 是当前的窗口 (HWND)
    if(!StretchBlt(hdcWindow, 
        0,0, 
        rcClient.right, rcClient.bottom, 
        hdcScreen, 
        0,0,
        GetSystemMetrics (SM_CXSCREEN),
        GetSystemMetrics (SM_CYSCREEN),
        SRCCOPY))
    {
        MessageBox(0,TEXT("StretchBlt has failed"),0,0);
        goto done;
    }
 
    // 通过窗口DC 创建一个兼容位图
    hbmScreen = CreateCompatibleBitmap(
        hdcWindow,
        rcClient.right-rcClient.left,
        rcClient.bottom-rcClient.top
        );
 
    if(!hbmScreen)
    {
        MessageBox(0,TEXT("CreateCompatibleBitmap Failed"),0,0);
        goto done;
    }
 
    // 将位图块传送到我们兼容的内存DC中
    SelectObject(hdcMemDC,hbmScreen);
    if(!BitBlt(
        hdcMemDC,   // 目的DC
        0,0,        // 目的DC的 x,y 坐标
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, // 目的 DC 的宽高
        hdcWindow,  // 来源DC
        0,0,        // 来源DC的 x,y 坐标
        SRCCOPY))   // 粘贴方式
    {
        MessageBox(0,TEXT("BitBlt has failed"),0,0);
        goto done;
    }
 
    // 获取位图信息并存放在 bmpScreen 中
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);
 
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;
 
    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
 
    // 在 32-bit Windows 系统上, GlobalAlloc 和 LocalAlloc 是由 HeapAlloc 封装来的
    // handle 指向进程默认的堆. 所以开销比 HeapAlloc 要大
    hDIB = GlobalAlloc(GHND,dwBmpSize); 
    lpbitmap = (char *)GlobalLock(hDIB);    
 
    // 获取兼容位图的位并且拷贝结果到一个 lpbitmap 中.
    GetDIBits(
        hdcWindow,  // 设备环境句柄
        hbmScreen,  // 位图句柄
        0,          // 指定检索的第一个扫描线
        (UINT)bmpScreen.bmHeight, // 指定检索的扫描线数
        lpbitmap,   // 指向用来检索位图数据的缓冲区的指针
        (BITMAPINFO *)&bi, // 该结构体保存位图的数据格式
        DIB_RGB_COLORS // 颜色表由红、绿、蓝（RGB）三个直接值构成
        );
 
 
    wsprintf(FilePath, "%s\\%s.bmp", dirPath, filename);
 
    // 创建一个文件来保存文件截图
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
 
    // 将 图片头(headers)的大小, 加上位图的大小来获得整个文件的大小
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    // 设置 Offset 偏移至位图的位(bitmap bits)实际开始的地方
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
 
    // 文件大小
    bmfHeader.bfSize = dwSizeofDIB; 
 
    // 位图的 bfType 必须是字符串 "BM"
    bmfHeader.bfType = 0x4D42; //BM   
 
    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
 
    // 解锁堆内存并释放
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);
 
    // 关闭文件句柄
    CloseHandle(hFile);
 
    // 清理资源
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(hwnd,hdcWindow);
 
    return 0;
}