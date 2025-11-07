#include <Windows.h>
#include <strsafe.h>
#include "resource.h"

// 视觉样式
// https://learn.microsoft.com/zh-cn/windows/win32/controls/cookbook-overview

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define THREADCOUNT 5

DWORD g_dwTlsIndex;
HWND g_hDlgWnd;

// 对话框过程函数
INT_PTR CALLBACK DialogProc(HWND hDlgWnd, UINT uMsg, WPARAM wPrama, LPARAM lParam);
// 线程函数
DWORD WINAPI ThreadProc(LPVOID lpParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDlgWnd, UINT uMsg, WPARAM wPrama, LPARAM lParam){
	HANDLE hThread[THREADCOUNT];

	switch (uMsg){
	case WM_INITDIALOG:
		g_hDlgWnd = hDlgWnd;
		break;
	case WM_COMMAND:
		switch (LOWORD(wPrama)){
			case IDOK:
			case IDCANCEL:
				EndDialog(hDlgWnd, IDCANCEL);
				return TRUE;
			case ID_CONFIRM: // 点击“确认”按钮
				g_dwTlsIndex = TlsAlloc(); // 分配 TLS 索引
				if (g_dwTlsIndex == TLS_OUT_OF_INDEXES) {
					MessageBox(hDlgWnd, TEXT("TlsAlloc 函数调用失败"), TEXT("错误提示"), MB_OK| MB_ICONERROR);
					return FALSE;
				}

				// 创建 THREADCOUNT 个线程
				SetDlgItemText(hDlgWnd, IDC_CONTENT, TEXT(""));
				for (int i = 0; i < THREADCOUNT; i++) {
					hThread[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(i + 1), 0, NULL);
					if (hThread[i] != NULL) {
						CloseHandle(hThread[i]);
						hThread[i] = NULL;
					}
				} 

				// 等待所有线程结束，释放TLS索引
				// 线程结束，线程对象会变成有信号状态
				// 如果对象是无信号状态，则调用线程进入等待状态
				WaitForMultipleObjects(THREADCOUNT, hThread, TRUE, INFINITE);
				TlsFree(g_dwTlsIndex);

				break;
		}
		break;
	}
	return FALSE;
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
	TCHAR szBuf[64] = { 0 };
	LPVOID data = NULL;
	data = new BYTE[256];	
	ZeroMemory(data, 256);

	// 注意，如果存储槽中该索引处存储的是一个内存块指针，该函数不会
	// 释放该内存块，释放内存块由程序员负责完成
	if (!TlsSetValue(g_dwTlsIndex, data)) {
		StringCchPrintf(szBuf, _countof(szBuf), TEXT("线程 %d: TlsSetValue 函数调用失败\r\n"), (INT)lpParam);
		MessageBox(g_hDlgWnd, szBuf, TEXT("错误提示"), MB_OK | MB_ICONERROR);
		delete[] data; // 释放内存
		return 0;
	}

	// 获取 TLS 数据,存储的可能是0值
	data = TlsGetValue(g_dwTlsIndex);
	if (!data && GetLastError() != ERROR_SUCCESS) {
		StringCchPrintf(szBuf, _countof(szBuf), TEXT("线程 %d: TlsGetValue 函数调用失败\r\n"), (INT)lpParam);
		MessageBox(g_hDlgWnd, szBuf, TEXT("错误提示"), MB_OK | MB_ICONERROR);
	}

	StringCchPrintf(szBuf, _countof(szBuf), TEXT("线程 %d: TLS 数据起始地址 = 0x%p\r\n"), (INT)lpParam, data);
	// 将结果显示在编辑框中
	// 追加写入
	SendDlgItemMessage(g_hDlgWnd, IDC_CONTENT, EM_SETSEL, -1, -1);
	SendDlgItemMessage(g_hDlgWnd, IDC_CONTENT, EM_REPLACESEL, TRUE, (LPARAM)szBuf);
	//SendMessage(GetDlgItem(g_hDlgWnd, IDC_CONTENT), EM_SETSEL, -1, -1);
	//SendMessage(GetDlgItem(g_hDlgWnd, IDC_CONTENT), EM_REPLACESEL, TRUE, (LPARAM)szBuf);

	delete[] data; // 释放内存
	return 0;
}