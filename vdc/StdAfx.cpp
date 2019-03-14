// stdafx.cpp : source file that includes just the standard includes
//	vdc.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


#include<stdio.h>
#include<iostream>
#include<string>
using   namespace   std;
#include<cstring>
#include "stdlib.h"
void MsgVal(long val){
		//int n=123456789;
		char str[10];
		itoa(val,str,10);
		OutputDebugStringA( str );
		OutputDebugStringA("\n\r");
		//MessageBox(0,str,0,0);
}
void MsgStr(TCHAR* str){
		//int n=123456789;
		OutputDebugStringA( str );
		OutputDebugStringA("\n\r");
		//MessageBox(0,str,0,0);
}
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
