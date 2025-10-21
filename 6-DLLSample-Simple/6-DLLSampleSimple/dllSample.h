#pragma once

// 声明导出的变量、函数和类
/*
* 在函数声明前面加上
* extern "C"关键字后，C++会对该函数强制使用标准C的函数名
* 称修饰方式，不会对函数名、变量名进行改编，在DLL中用这
* 种方式导出的函数可以被其他语言使用；
*/
#ifdef DLL_EXPORT
    #define DLLSAMPLE_API       extern "C" __declspec(dllexport)
#else
    #define DLLSAMPLE_API       extern "C" __declspec(dllimport)
#endif


// 导出函数
/*
* 当使用__stdcall来导出C函数时，编译器会对函数名进行改编，
* 函数名前面有一个下划线“_”，然后是函数名，函数名后面是一
* 个@符号后跟作为参数传递给函数的字节数组成
* (x86 平台测试会对函数名进行改编，x64不会改编!)
*   _funAdd@8
*   _funMul@8
* 
* 这种情况下，我们可以添加一个.def文件指示编译器导出
* funAdd和funMul函数，而不是_funAdd@8和_funMul@8函数。
*/
DLLSAMPLE_API int WINAPI funAdd(int a, int b);
DLLSAMPLE_API int WINAPI funMul(int a, int b);