#pragma once

// ���������ı�������������
/*
* �ں�������ǰ�����
* extern "C"�ؼ��ֺ�C++��Ըú���ǿ��ʹ�ñ�׼C�ĺ�����
* �����η�ʽ������Ժ����������������иı࣬��DLL������
* �ַ�ʽ�����ĺ������Ա���������ʹ�ã�
*/
#ifdef DLL_EXPORT
    #define DLLSAMPLE_API       extern "C" __declspec(dllexport)
#else
    #define DLLSAMPLE_API       extern "C" __declspec(dllimport)
#endif


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