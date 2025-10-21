#pragma once

// 声明导出的变量、函数和类
#ifdef DLL_EXPORT
    #define DLLSAMPLE_VARABLE   extern "C" __declspec(dllexport)
    #define DLLSAMPLE_CLASS     __declspec(dllexport)
    #define DLLSAMPLE_API       extern "C" __declspec(dllexport)
#else
    #define DLLSAMPLE_VARABLE   extern "C" __declspec(dllimport)
    #define DLLSAMPLE_CLASS     __declspec(dllimport)
    #define DLLSAMPLE_API       extern "C" __declspec(dllimport)
#endif

typedef struct _POSITION
{
	int x;
	int y;
}POSITION, * PPOSITION;

// 导出变量
DLLSAMPLE_VARABLE int nValue;   // 导出普通变量
DLLSAMPLE_VARABLE POSITION ps;  // 导出结构体变量

// 导出类
class DLLSAMPLE_CLASS CStudent
{
    
public:
	CStudent(LPTSTR lpName, int nAge);
	~CStudent();

public:
	LPTSTR  GetName();
	int     GetAge();

private:
	TCHAR   m_szName[64];
	int     m_nAge;
};

// 导出函数
DLLSAMPLE_API int WINAPI funAdd(int a, int b);
DLLSAMPLE_API int WINAPI funMul(int a, int b);