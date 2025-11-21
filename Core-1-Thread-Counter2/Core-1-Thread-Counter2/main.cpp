#include <Windows.h>
#include "resource.h"

// 视觉样式
// https://learn.microsoft.com/zh-cn/windows/win32/controls/cookbook-overview

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HANDLE g_hEventStart;	// 事件对象，作为开始
HANDLE g_hEventStop;	// 事件对象，作为停止

HWND g_hwndDlg;

// 对话框窗口过程函数声明
INT_PTR CALLBACK DialogProc(HWND hDiaWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 计数函数声明
DWORD WINAPI Counter(LPVOID lpParameter);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR pCmdline, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDiaWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndBtnStart, hwndBtnPause, hwndBtnStop, hwndBtnContinue;
	HANDLE hThread = NULL;

	switch (uMsg) {
		case WM_INITDIALOG: {
			g_hwndDlg = hDiaWnd;

			hwndBtnStart = GetDlgItem(hDiaWnd, IDC_START);
			hwndBtnContinue = GetDlgItem(hDiaWnd, IDC_CONTINUE);
			hwndBtnPause = GetDlgItem(hDiaWnd, IDC_PAUSE);
			hwndBtnStop = GetDlgItem(hDiaWnd, IDC_STOP);

			// 创建事件对象
			g_hEventStart = CreateEvent(NULL, TRUE, FALSE, NULL);
			g_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);

			EnableWindow(hwndBtnContinue, FALSE);
			EnableWindow(hwndBtnPause, FALSE);
			EnableWindow(hwndBtnStop, FALSE);
			return TRUE;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					// 关闭事件对象句柄
					CloseHandle(g_hEventStart);
					CloseHandle(g_hEventStop);
					EndDialog(hDiaWnd, 0);
					break;
				case IDC_START: { // 开始按钮
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, TRUE);
					EnableWindow(hwndBtnStop, TRUE);

					hThread = CreateThread(NULL, 0, Counter, NULL, 0, NULL);
					// 关闭线程句柄避免句柄泄漏
					if (hThread != NULL) {
						CloseHandle(hThread);
						hThread = NULL;
					}

					SetEvent(g_hEventStart);	// 设置开始标志，有状态
					ResetEvent(g_hEventStop);	// 清除停止标志，没状态
					break;
				}
				case IDC_PAUSE: { // 暂停按钮
					ResetEvent(g_hEventStart);	// 清除开始标志
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, TRUE);
					EnableWindow(hwndBtnPause, FALSE);
					EnableWindow(hwndBtnStop, TRUE);
					break;
				}
				case IDC_STOP: { // 停止按钮
					SetEvent(g_hEventStop);	// 设置停止标志
					EnableWindow(hwndBtnStart, TRUE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, FALSE);
					EnableWindow(hwndBtnStop, FALSE);
					SetDlgItemInt(g_hwndDlg, IDC_COUNTER, 0, FALSE);
					break;
				}
				case IDC_CONTINUE: { // 继续
					SetEvent(g_hEventStart);	// 设置开始标志
					EnableWindow(hwndBtnStart, FALSE);
					EnableWindow(hwndBtnContinue, FALSE);
					EnableWindow(hwndBtnPause, TRUE);
					EnableWindow(hwndBtnStop, TRUE);
					break;
				}
			}
		}
	}
	return FALSE;
}

/*
*
* 在主线程中执行循环，会导致程序无法响应。
* 因此，将计数功能放在单独的线程中执行，以保持界面的响应性。
*
*/
DWORD WINAPI Counter(LPVOID lpParameter) {
	int count = 0;
	// 可以把事件对象看作一个由Windows管理的标志，事件对象
	// 有两种状态：有信号和无信号状态，也称为触发和未触发状态。

	// WaitForSingleObject 函数检查指定对象的当前状态(不同对象对状态的定义是不同的)。
	// 用于等待指定的对象变成有信号状态
	// 如果对象的状态不对齐，则调用线程将进入等待状态，
	// 直到发出该对象的信号或超时间隔结束。

	// @return 
	// WAIT_OBJECT_0 等待的对象变成有信号状态
	// WAIT_TIMEOUT 超时时间已过
	// WAIT_FAILED 函数执行失败

	while (WaitForSingleObject(g_hEventStop, 0) != WAIT_OBJECT_0) {	// 是否设置了停止标志
		// 是否设置了开始标志
		if (WaitForSingleObject(g_hEventStart, 100) == WAIT_OBJECT_0) {
			SetDlgItemInt(g_hwndDlg, IDC_COUNTER, count++, FALSE);
		}
	}
	return 0;
}