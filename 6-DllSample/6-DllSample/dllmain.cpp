// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <strsafe.h>
#include <cstdlib>

int nValue;
POSITION ps;

// DLL 入口点函数
/*
* 一个DLL可以有一个入口点函数DllMain（区分大小写），系统
* 会在不同的时刻调用这个入口点函数，当系统装载、卸载动态
* 链接库，以及进程中有线程被创建、退出时，系统会调用入口
* 点函数。系统调用入口点函数是通知性质，通知DLL执行一些
* 与进程或线程有关的初始化和清理工作，如果DLL不需要这些
* 通知，可以不必在源代码中实现该入口点函数，例如要创建
* 一个只包含资源的DLL，则不需要实现该函数
*/
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		// DLL 正在被映射到进程的地址空间中
        nValue = 5;
        ps.x = 6;
        ps.y = 7;
        break;
    case DLL_PROCESS_DETACH:
		// DLL 正从进程的地址空间中卸载
        break;
    case DLL_THREAD_ATTACH:
		// 进程中创建了一个新的线程
        break;
    case DLL_THREAD_DETACH:
		// 进程中有一个线程正在退出
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

