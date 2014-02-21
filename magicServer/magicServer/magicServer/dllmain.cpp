// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "magicServer_i.h"
#include "dllmain.h"

CmagicServerModule _AtlModule;

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
