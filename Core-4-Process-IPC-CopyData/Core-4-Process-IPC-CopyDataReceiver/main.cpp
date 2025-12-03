#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"
#include "../Core-4-Process-IPC-CopyData/copydata.h"

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
				case IDCANCEL:
				case IDOK:
					EndDialog(hWnd, 0);
					break;
			}
			break;
		}
		case WM_COPYDATA: {
			PCOPYDATASTRUCT pCopyDataStruct = (PCOPYDATASTRUCT)lParam;
			TCHAR szBuf[256] = { 0 };
			if (pCopyDataStruct->dwData == PERSONDATA) {
				PPERSONSTRUCT pPS = (PPERSONSTRUCT)pCopyDataStruct->lpData;
				StringCchPrintf(szBuf, sizeof(szBuf), TEXT("姓名:%s\r\n年龄:%d\r\n存款:%.2f"), 
					pPS->m_szName, pPS->m_nAge, pPS->m_dDeposit);
				MessageBox(hWnd, szBuf, TEXT("个人信息"), MB_OK);
			} else if (pCopyDataStruct->dwData == SCOREDATA) {
				PSCORESTRUCT pSS = (PSCORESTRUCT)pCopyDataStruct->lpData;
				StringCchPrintf(szBuf, sizeof(szBuf), TEXT("语文成绩:%.1f\r\n数学成绩:%.1f\r\n英语成绩:%.1f"), 
					pSS->m_fChiniess, pSS->m_fMath, pSS->m_fEnglish);
				MessageBox(hWnd, szBuf, TEXT("考试成绩"), MB_OK);
			}
			break;
		}
	}
	return FALSE;
}