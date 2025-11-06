#include <Windows.h>
#include <Shlwapi.h>
#include "resource.h"
#include "GetMd5.h"

#pragma comment(lib, "6-GetMd5Dll.lib")

// 视觉样式
// https://learn.microsoft.com/zh-cn/windows/win32/controls/cookbook-overview

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {

	// 对话框窗口过程
	INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDiaWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDROP hDrop;
	TCHAR szFilePath[MAX_PATH] = {0};
	TCHAR md5[64] = { 0 };

	switch (uMsg) {
	case WM_INITDIALOG: {
		// 初始化工作
		SendDlgItemMessage(hDiaWnd, IDC_EDIT1, EM_SETCUEBANNER, TRUE, (LPARAM)TEXT("输入文件路径或拖动文件到此窗口"));
		break;
	}
	case WM_CREATE:

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hDiaWnd, 0);
			break;
		case IDC_BUTTON1:
			// 获取MD5按钮被按下
			int success = GetDlgItemText(hDiaWnd, IDC_EDIT1, szFilePath, _countof(szFilePath));
			if (success) {
				if (GetMd5(szFilePath, md5)) {
					SetDlgItemText(hDiaWnd, IDC_STATIC_MD5, md5);
				}else {
					MessageBox(hDiaWnd, TEXT("获取MD5失败，请检查文件路径是否正确！"), TEXT("错误"), MB_OK | MB_ICONERROR)	;
				}
			}
			break;
		}
		return TRUE;
	case WM_DROPFILES:
		hDrop = (HDROP)wParam;
		// 查询所拖放文件的名称
		DragQueryFile(hDrop, 0, szFilePath, _countof(szFilePath));
		SetDlgItemText(hDiaWnd, IDC_EDIT1, szFilePath);
		// 或者使用 SendDlgItemMessage 设置文本也可以
		//SendDlgItemMessage(hDiaWnd, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)szFilePath);
		DragFinish(hDrop);
		return TRUE;
	}

	return FALSE;
}