// 6-StaticLib1.cpp : 定义静态库的函数。
//
/*
* 静态链接库仅
* 在编译链接时使用，编译完成后，可执行文件即可脱离库文件
* 单独使用
*/

#include "pch.h"
#include "framework.h"

int funAdd(int a, int b)
{
	return a + b;
}

int funMul(int a, int b)
{
	return a * b;
}
