// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <strsafe.h>
#include <cstdlib>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


// 函数
int WINAPI funAdd(int a, int b)
{
    return a + b;
}

int WINAPI funMul(int a, int b)
{
    return a * b;
}

