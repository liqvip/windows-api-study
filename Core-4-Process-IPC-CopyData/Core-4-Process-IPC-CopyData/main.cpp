#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "copydata.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdline, int nCmdShow) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_SEND_INFO: { // 发送个人信息
					TCHAR lpWindowName[32] = TEXT("接收数据");
					HWND hWndDest;
					if ((hWndDest = FindWindow(NULL, lpWindowName)) != NULL) {
						// 获取一下目标窗口的进程ID
						DWORD dwDestProcessId;
						GetWindowThreadProcessId(hWndDest, &dwDestProcessId);

						PERSONSTRUCT person = {0};
						GetDlgItemText(hWnd, IDC_NAME, person.m_szName, _countof(person.m_szName));
						person.m_nAge = GetDlgItemInt(hWnd, IDC_AGE, NULL, FALSE);
						TCHAR lpString[32] = { 0 };
						GetDlgItemText(hWnd, IDC_DEPOSIT, lpString, _countof(lpString));
						person.m_dDeposit = _ttof(lpString);
						// 不能使用 PostMessage, SendNotifyMessage, SendMessageCallback 函数
						// 这些函数的 wParam 和 lParam 参数中不能传递指针，因为这些函数立即返回，函数返回后指
						// 针指向的内存会被释放，函数调用会失败
						COPYDATASTRUCT copyDataStrct = {0};
						
						copyDataStrct.dwData = PERSONDATA;
						copyDataStrct.cbData = sizeof(PERSONSTRUCT);
						copyDataStrct.lpData = &person;
						SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndDest, (LPARAM)&copyDataStrct);
					}
					break;
				}
				case IDC_SEND_SCORE: {	// 发送分数	
					TCHAR lpWindowName[32] = TEXT("接收数据");
					HWND hWndDest;
					if ((hWndDest = FindWindow(NULL, lpWindowName)) != NULL) {
						// 获取一下目标窗口的进程ID
						DWORD dwDestProcessId;
						GetWindowThreadProcessId(hWndDest, &dwDestProcessId);

						SCORESTRUCT score = { 0 };

						// 语文成绩
						TCHAR lpScore[32] = { 0 };
						GetDlgItemText(hWnd, IDC_SCORE_CHINESS, lpScore, _countof(lpScore));
						score.m_fChiniess = _ttof(lpScore);

						// 数学成绩
						ZeroMemory(lpScore, _countof(lpScore) * sizeof(TCHAR));
						GetDlgItemText(hWnd, IDC_SCORE_MATH, lpScore, _countof(lpScore));
						score.m_fMath = _ttof(lpScore);

						// 英语成绩
						ZeroMemory(lpScore, _countof(lpScore) * sizeof(TCHAR));
						GetDlgItemText(hWnd, IDC_SCORE_ENGLISH, lpScore, _countof(lpScore));
						score.m_fEnglish = _ttof(lpScore);

						// 不能使用 PostMessage, SendNotifyMessage, SendMessageCallback 函数
						// 这些函数的 wParam 和 lParam 参数中不能传递指针，因为这些函数立即返回，函数返回后指
						// 针指向的内存会被释放，函数调用会失败
						COPYDATASTRUCT copyDataStrct = { 0 };

						copyDataStrct.dwData = SCOREDATA;
						copyDataStrct.cbData = sizeof(SCORESTRUCT);
						copyDataStrct.lpData = &score;
						SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndDest, (LPARAM)&copyDataStrct);
					}
					break;
				}
				case IDCANCEL:
				case IDOK:
					EndDialog(hWnd, 0);
					break;
			}
			break;
		}
	}
	return FALSE;
}