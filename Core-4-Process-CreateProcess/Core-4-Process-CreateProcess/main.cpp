#include <Windows.h>
#include "resource.h"
#include <strsafe.h>

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static TCHAR szInfo[512] = { 0 };
	switch (uMsg) {
		case WM_INITDIALOG: {
			__int64 baseAddress = (__int64)GetModuleHandle(NULL);
			DWORD dwID = GetCurrentProcessId();
			StringCchPrintf(szInfo, _countof(szInfo), TEXT("当前进程ID:%d\r\n当前进程基地址:0X%X\r\n"), dwID, baseAddress);
			SetDlgItemText(hWnd, IDC_INFO, szInfo);
			return TRUE;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL: {
					EndDialog(hWnd, 0);
					break;
				}
				case IDC_OPEN_CALC: {
					TCHAR szCmdLine[MAX_PATH] = TEXT("calc");
					STARTUPINFO si = { sizeof(STARTUPINFO)};
					PROCESS_INFORMATION pi = { 0 };
					GetStartupInfo(&si);
					if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
						// 挂起调用线程,等待子进程初始化完毕
						DWORD dwRes= WaitForInputIdle(pi.hProcess, 5000);
						// 获取计算器父进程ID
						TCHAR szDest[256] = { 0 };
						StringCchPrintf(szDest, _countof(szDest), TEXT("计算器进程ID:%d\r\n计算机线程ID:%d\r\n"), pi.dwProcessId, pi.dwThreadId);
						StringCchCat(szInfo, _countof(szInfo), szDest);
						SetDlgItemText(hWnd, IDC_INFO, szInfo);
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
					}
					break;
				}
			}
			break;
		}
	}
	return FALSE;
}
