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
	static HWND hWndAge, hWndName, hWndDeposit, hWndSendInfo, hWndScoreChiness, hWndScoreMath, hWndScoreEnglish, hWndSendScore;
	switch (uMsg) {
		case WM_INITDIALOG: {
			hWndAge = GetDlgItem(hWnd, IDC_AGE);
			hWndName = GetDlgItem(hWnd, IDC_NAME);
			hWndDeposit = GetDlgItem(hWnd, IDC_DEPOSIT);
			hWndSendInfo = GetDlgItem(hWnd, IDC_SEND_SCORE);

			hWndScoreChiness = GetDlgItem(hWnd, IDC_SCORE_CHINESS);
			hWndScoreMath = GetDlgItem(hWnd, IDC_SCORE_MATH);
			hWndScoreEnglish = GetDlgItem(hWnd, IDC_SCORE_ENGLISH);
			hWndSendScore = GetDlgItem(hWnd, IDC_SEND_SCORE);
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
						GetDlgItemText(hWndName, IDC_NAME, person.m_szName, _countof(person.m_szName));
						person.m_nAge = GetDlgItemInt(hWndAge, IDC_AGE, NULL, FALSE);
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