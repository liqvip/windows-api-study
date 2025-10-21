#pragma once

// ���������ı�������������
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

// ��������
DLLSAMPLE_VARABLE int nValue;   // ������ͨ����
DLLSAMPLE_VARABLE POSITION ps;  // �����ṹ�����

// ������
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

// ��������
DLLSAMPLE_API int WINAPI funAdd(int a, int b);
DLLSAMPLE_API int WINAPI funMul(int a, int b);