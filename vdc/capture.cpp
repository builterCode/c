#include "capture.h"

///< ö�ٴ��ڲ���
typedef struct
{
    HWND    hwndWindow;     // ���ھ��
    DWORD   dwProcessID;    // ����ID
}EnumWindowsArg;
///< ö�ٴ��ڻص�����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    EnumWindowsArg *pArg = (EnumWindowsArg *)lParam;
    DWORD  dwProcessID = 0;
    // ͨ�����ھ��ȡ�ý���ID
    GetWindowThreadProcessId(hwnd, &dwProcessID);
    if (dwProcessID == pArg->dwProcessID)
    {
        pArg->hwndWindow = hwnd;
        // �ҵ��˷���FALSE
        return FALSE;
    }
    // û�ҵ��������ң�����TRUE
    return TRUE;
}
///< ͨ������ID��ȡ���ھ��
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

///< ͨ������ID��ȡ���ھ��
HWND GetWindowHwndByPID2(DWORD dwProcessID)
{
    DWORD dwPID = 0;
    HWND hwndRet = NULL;
    // ȡ�õ�һ�����ھ��
    HWND hwndWindow = ::GetTopWindow(0);
    while (hwndWindow)
    {
        dwPID = 0;
        // ͨ�����ھ��ȡ�ý���ID
        DWORD dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPID);
        if (dwTheardID != 0)
        {
            // �жϺͲ�������Ľ���ID�Ƿ����
            if (dwPID == dwProcessID)
            {
                // ����ID��ȣ����¼���ھ��
                hwndRet = hwndWindow;
                break;
            }
        }
        // ȡ����һ�����ھ��
        hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT);
    }
    // ����ȡ�õĴ��ڣ���һ�������ϲ�Ĵ��ڣ���Ҫͨ��GetParent��ȡ��㴰��
    HWND hwndWindowParent = NULL;
    // ѭ�����Ҹ����ڣ��Ա㱣֤���صľ�������Ĵ��ھ��
    while (hwndRet != NULL)
    {
        hwndWindowParent = ::GetParent(hwndRet);
        if (hwndWindowParent == NULL)
        {
            break;
        }
        hwndRet = hwndWindowParent;
    }
    // ���ش��ھ��
    return hwndRet;
}

/*-------------------------------------------------------
FindWindow()����һ�ֲ�©�����������Ĵ��ڱ������ƣ�������Ҳ���Ŀ�괰�ڣ��������ǽ�һ������ GetWindow()��ʽ����Ѱȫ���ɼ����ڣ�����һ�ȶ��䴰�ڱ������ƣ������Ϳ�������ν��������Ѱ�������µĳ������Ὣ���������д��ڱ������ƺ��С����±��������Ĵ���ͨͨ�ر� 
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
    //����һ
    int i = str1.find(str2);  //���ؼ����ַ�������3
    //������
    char *rel = strstr(a, b); //�״γ��ֵ�ַ��strstr�������ddabc
    if (rel != NULL)
        j = rel -a;         //���ݷ������ַ���ƥ�����������λ

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
						SendMessage(hCurrentWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0); // ���
						return hCurrentWindow;
                       // ��С��ָ���ĳ���
                        /*
                        CloseWindow(hCurrentWindow); (����Ϊ���ǡ�������ָ��������÷�)
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
 * GDI ��������
 * ���� hwnd   Ҫ�����Ĵ��ھ��
 * ���� dirPath    ��ͼ���Ŀ¼
 * ���� filename ��ͼ����
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
    INT width = GetSystemMetrics(SM_CXSCREEN);  // ��Ļ��
    INT height = GetSystemMetrics(SM_CYSCREEN);  // ��Ļ��
    HDC hdcScreen = GetDC(NULL); // ȫ��ĻDC
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // ���������ڴ�DC
 
	RECT rcClient;
	// ��ȡ�ͻ����������ڼ����С
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

    // ͨ������DC ����һ������λͼ
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
    // ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
    SelectObject(hdcMemDC, hbmScreen);
    if (!BitBlt(
        hdcMemDC,    // Ŀ��DC
        0,0,        // Ŀ��DC�� x,y ����
        pic_width,pic_height, // Ŀ�� DC �Ŀ��
        hdcScreen,   // ��ԴDC
        rcClient.left, rcClient.top,        // ��ԴDC�� x,y ����
        SRCCOPY))    // ճ����ʽ
    {
        DeleteObject(hbmScreen);
        DeleteObject(hdcMemDC);
        ReleaseDC(NULL, hdcScreen);
		MsgStr("exit in BitBlt");
        return 0;
    }

    // ��ȡλͼ��Ϣ������� bmpScreen ��
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

    // �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
    // handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char *)GlobalLock(hDIB);

    // ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
    GetDIBits(
        hdcScreen,  // �豸�������
        hbmScreen,  // λͼ���
        0,          // ָ�������ĵ�һ��ɨ����
        (UINT)bmpScreen.bmHeight, // ָ��������ɨ������
        lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
        (BITMAPINFO *)&bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
        DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
    );


    wsprintf(FilePath, "%s\\%s.jpg", dirPath, filename);

    // ����һ���ļ��������ļ���ͼ
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // �ļ���С
    bmfHeader.bfSize = dwSizeofDIB;

    // λͼ�� bfType �������ַ��� "BM"
    bmfHeader.bfType = 0x4D42; //BM

    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    // �������ڴ沢�ͷ�
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    // �ر��ļ����
    CloseHandle(hFile);

    // ������Դ
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);

	MsgStr("pic saved");
    return 0;
}


/**
 * GDI ��ȡָ������
 * 
 * ���� hwnd   Ҫ�����Ĵ��ھ��
 * ���� FilePath    ��ͼ���Ŀ¼
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

    hdcWindow = GetWindowDC(hwnd); // ��ͼĿ�괰��DC
//    hdcWindow = GetDC(hwnd); // ��ͼDC
 
    // ���������ڴ�DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 
 
    if(!hdcMemDC)
    {
        MessageBox(0,"CreateCompatibleDC has failed",0,0);
        goto done;
    }
 
    // ��ȡ�ͻ����������ڼ����С
    GetClientRect(hwnd, &rcClient);
 
    // ������չģʽ
    SetStretchBltMode(hdcWindow, HALFTONE);
 
    // ��Դ DC ��������Ļ��Ŀ�� DC �ǵ�ǰ�Ĵ��� (HWND)
/*    if(!StretchBlt(hdcWindow,                      //-----------ע��ԭ�򣺲���Ҫѹ��λͼ���ֲ���ȡȫ��
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

    // ͨ������DC ����һ������λͼ
    hbmScreen = CreateCompatibleBitmap(
        hdcWindow,
        width, height//--λͼ���
        );
 
    if(!hbmScreen)
    {
        MessageBox(0,"CreateCompatibleBitmap Failed",0,0);
        goto done;
    }
 
    // ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
    SelectObject(hdcMemDC,hbmScreen);
    //---------------------------ע��ԭ�򣺽�ȡ������ֽ��棬��Ҫ�趨��ԴDC�������Լ�Ŀ��DC�Ŀ�ߣ�����ԭ�����Թ���  
    if(!BitBlt(
        hdcMemDC,   // Ŀ��DC
        0,0,        // Ŀ��DC�� x,y ����
        width,height, // Ŀ�� DC �Ŀ��
        hdcWindow,  // ��ԴDC
        0,0,        // ��ԴDC�� ���
        SRCCOPY))   // ճ����ʽ
    {
        MessageBox(0,"BitBlt has failed",0,0);
        goto done;
    }
      /*
    if(!BitBlt(
        hdcMemDC,   // Ŀ��DC
        0,0,        // Ŀ��DC�� x,y ����
        400, 300, // Ŀ�� DC �Ŀ��
        hdcWindow,  // ��ԴDC
        50,50+26,        // ��ԴDC�Ŀ��
        SRCCOPY))   // ճ����ʽ
    {
        MessageBox(0,"BitBlt has failed",0,0);
        goto done;
    }
 */
    // ��ȡλͼ��Ϣ������� bmpScreen ��
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
 
    // �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
    // handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
    hDIB = GlobalAlloc(GHND,dwBmpSize); 
    lpbitmap = (char *)GlobalLock(hDIB);    
 
    // ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
    GetDIBits(
        hdcWindow,  // �豸�������
        hbmScreen,  // λͼ���
        0,          // ָ�������ĵ�һ��ɨ����
        (UINT)bmpScreen.bmHeight, // ָ��������ɨ������
        lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
        (BITMAPINFO *)&bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
        DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
        );
 
 
 
    // ����һ���ļ��������ļ���ͼ
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
 
    // �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    // ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
 
    // �ļ���С
    bmfHeader.bfSize = dwSizeofDIB; 
 
    // λͼ�� bfType �������ַ��� "BM"
    bmfHeader.bfType = 0x4D42; //BM   
 
    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
 
    // �������ڴ沢�ͷ�
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);
 
    // �ر��ļ����
    CloseHandle(hFile);
 
    // ������Դ
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(hwnd,hdcWindow);
 
    return 0;
}


/**
 * GDI ��ȡָ������
 * 
 * ���� hwnd   Ҫ�����Ĵ��ھ��
 * ���� dirPath    ��ͼ���Ŀ¼
 * ���� filename ��ͼ����
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
 
    hdcScreen = GetDC(NULL); // ȫ��ĻDC
    hdcWindow = GetDC(hwnd); // ��ͼĿ�괰��DC
 
    // ���������ڴ�DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 
 
    if(!hdcMemDC)
    {
        MessageBox(0,TEXT("CreateCompatibleDC has failed"),0,0);
        goto done;
    }
 
    // ��ȡ�ͻ����������ڼ����С
    GetClientRect(hwnd, &rcClient);
 
    // ������չģʽ
    SetStretchBltMode(hdcWindow, HALFTONE);
 
    // ��Դ DC ��������Ļ��Ŀ�� DC �ǵ�ǰ�Ĵ��� (HWND)
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
 
    // ͨ������DC ����һ������λͼ
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
 
    // ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
    SelectObject(hdcMemDC,hbmScreen);
    if(!BitBlt(
        hdcMemDC,   // Ŀ��DC
        0,0,        // Ŀ��DC�� x,y ����
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, // Ŀ�� DC �Ŀ��
        hdcWindow,  // ��ԴDC
        0,0,        // ��ԴDC�� x,y ����
        SRCCOPY))   // ճ����ʽ
    {
        MessageBox(0,TEXT("BitBlt has failed"),0,0);
        goto done;
    }
 
    // ��ȡλͼ��Ϣ������� bmpScreen ��
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
 
    // �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
    // handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
    hDIB = GlobalAlloc(GHND,dwBmpSize); 
    lpbitmap = (char *)GlobalLock(hDIB);    
 
    // ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
    GetDIBits(
        hdcWindow,  // �豸�������
        hbmScreen,  // λͼ���
        0,          // ָ�������ĵ�һ��ɨ����
        (UINT)bmpScreen.bmHeight, // ָ��������ɨ������
        lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
        (BITMAPINFO *)&bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
        DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
        );
 
 
    wsprintf(FilePath, "%s\\%s.bmp", dirPath, filename);
 
    // ����һ���ļ��������ļ���ͼ
    hFile = CreateFile(
        FilePath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
 
    // �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    // ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
 
    // �ļ���С
    bmfHeader.bfSize = dwSizeofDIB; 
 
    // λͼ�� bfType �������ַ��� "BM"
    bmfHeader.bfType = 0x4D42; //BM   
 
    dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
 
    // �������ڴ沢�ͷ�
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);
 
    // �ر��ļ����
    CloseHandle(hFile);
 
    // ������Դ
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(hwnd,hdcWindow);
 
    return 0;
}