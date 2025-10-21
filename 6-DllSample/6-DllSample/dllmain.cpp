// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <strsafe.h>
#include <cstdlib>

int nValue;
POSITION ps;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        nValue = 5;
        ps.x = 6;
        ps.y = 7;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// 类
CStudent::CStudent(LPTSTR lpName, int nAge)
{
    if (m_szName)
        StringCchCopy(m_szName, _countof(m_szName), lpName);

    m_nAge = nAge;
}

CStudent::~CStudent()
{
}

LPTSTR CStudent::GetName()
{
    return m_szName;
}

int    CStudent::GetAge()
{
    return m_nAge;
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

