#pragma once

// ���������ı�������������
/*
* �ں�������ǰ�����
* extern "C"�ؼ��ֺ�C++��Ըú���ǿ��ʹ�ñ�׼C�ĺ�����
* �����η�ʽ������Ժ����������������иı࣬��DLL������
* �ַ�ʽ�����ĺ������Ա���������ʹ�ã�
*/
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
/*
* ��ʹ��__stdcall������C����ʱ����������Ժ��������иı࣬
* ������ǰ����һ���»��ߡ�_����Ȼ���Ǻ�������������������һ
* ��@���ź����Ϊ�������ݸ��������ֽ������
* (x86 ƽ̨���Ի�Ժ��������иı࣬x64����ı�!)
*   _funAdd@8
*   _funMul@8
* 
* ��������£����ǿ������һ��.def�ļ�ָʾ����������
* funAdd��funMul������������_funAdd@8��_funMul@8������
*/
DLLSAMPLE_API int WINAPI funAdd(int a, int b);
DLLSAMPLE_API int WINAPI funMul(int a, int b);