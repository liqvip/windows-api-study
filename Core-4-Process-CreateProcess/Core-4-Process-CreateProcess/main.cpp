#include <Windows.h>
#include "resource.h"
#include <strsafe.h>
#include <string>

using namespace std;
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static wstring res;
	switch (uMsg) {
		case WM_INITDIALOG: {
			__int64 baseAddress = (__int64)GetModuleHandle(NULL);
			// 		baseAddress	0x00007ff7f4e80000	__int64
			DWORD dwID = GetCurrentProcessId();
			res = L"当前进程ID:" + to_wstring(dwID) + L"\r\n" + L"当前进程基地址:0x" + to_wstring(baseAddress) + L"\r\n";
			SetDlgItemText(hWnd, IDC_INFO, res.c_str());
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
						wstring szDest;
						res += L"计算器进程ID:" + to_wstring(pi.dwProcessId) + L"\r\n" + L"计算器线程ID:" + to_wstring(pi.dwThreadId) + L"\r\n";
						SetDlgItemText(hWnd, IDC_INFO, res.c_str());
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
