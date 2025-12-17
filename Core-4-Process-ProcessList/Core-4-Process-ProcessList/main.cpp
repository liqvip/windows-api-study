#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#include <Commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


HINSTANCE g_hInstance;
HWND g_hWnd;
HIMAGELIST g_hImageListSmall;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 显示进程列表
BOOL GetProcessList();

// 暂停、恢复进程
VOID SuspendProcess(DWORD dwProcessId, BOOL bSuspend);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow){
	g_hInstance = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN lvc = { 0 };
	POINT pt = {0};
	int nSelected, nRet;
	LVITEM lvi = { 0 };
	
	TCHAR szProcessName[MAX_PATH] = { 0 }, szProcessID[16] = { 0 }, szBuf[260] = { 0 };
	HANDLE hProcess;
	HMENU hMenu;
	BOOL bRet = FALSE;

	switch (uMsg) {
		case WM_INITDIALOG: {
			g_hWnd = hWnd;
			// 发送消息设置列表视图控件的扩展样式
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

			// 报表视图样式,需要发送 LVM_INSERTCOLUMN 消息来添加列标题。
			lvc.mask = LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT; // 列编号,列宽度,列标题有效
			lvc.iSubItem = 0; lvc.cx = 150; lvc.pszText = TEXT("进程名称");
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

			lvc.iSubItem = 1; lvc.cx = 60; lvc.pszText = TEXT("进程ID");
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

			lvc.iSubItem = 2; lvc.cx = 60; lvc.pszText = TEXT("父进程ID");
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_INSERTCOLUMN, 2, (LPARAM)&lvc);

			lvc.iSubItem = 3; lvc.cx = 260; lvc.pszText = TEXT("可执行文件路径");
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_INSERTCOLUMN, 3, (LPARAM)&lvc);

			// 为列表视图控件设置图像列表
			g_hImageListSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
				ILC_MASK | ILC_COLOR32, 500, 0);
			SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)g_hImageListSmall);

			// 显示进程列表
			GetProcessList();

			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case IDOK:
			case IDCANCEL:
				ImageList_Destroy(g_hImageListSmall);
				EndDialog(hWnd, 0);
				break;
			case ID_REFRESH_PROCESS: {	// 刷新进程列表
				GetProcessList();
				break;
			}
			case ID_END_PROCESS: {	// 结束进程
				nSelected = SendMessage(GetDlgItem(g_hWnd, IDC_LIST),LVM_GETSELECTIONMARK, 0, 0);
				// 确定要结束进程吗
				lvi.iItem = nSelected; lvi.iSubItem	= 0;lvi.mask = LVIF_TEXT;
				lvi.pszText = szProcessName; lvi.cchTextMax = _countof(szProcessName);
				SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETITEM, 0, (LPARAM)&lvi);

				StringCchPrintf(szBuf, _countof(szBuf), TEXT("确定要结束 %s 进程吗？"), lvi.pszText);
				nRet = MessageBox(hWnd, szBuf, TEXT("结束进程"), MB_OKCANCEL);
				if (nRet == IDCANCEL) return FALSE;

				lvi.iItem = nSelected; lvi.iSubItem	= 1;lvi.mask = LVIF_TEXT;
				lvi.pszText = szProcessID; lvi.cchTextMax = _countof(szProcessID);
				SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETITEM, 0, (LPARAM)&lvi);
				hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _ttoi(lvi.pszText));

				if (hProcess) {
					bRet = TerminateProcess(hProcess, 0);
					if (!bRet) {
						StringCchPrintf(szBuf, _countof(szBuf), TEXT("结束进程 %s 失败"), szProcessName);
						MessageBox(hWnd, szBuf, TEXT("错误提示"), MB_OK);
					}else {
						// 删除列表项
						SendDlgItemMessage(hWnd, IDC_LIST, LVM_DELETEITEM, nSelected, 0);
					}
					CloseHandle(hProcess);
				}
				break;
			}
			case ID_OPEN_LOCATION: { // 打开文件所在位置
				nSelected = SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETSELECTIONMARK, 0, 0);
				lvi.iItem = nSelected;	lvi.iSubItem = 3;	lvi.mask = LVIF_TEXT;	
				lvi.pszText = szProcessName;	lvi.cchTextMax = _countof(szProcessName);
				SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETITEM, 0, (LPARAM)&lvi);

				StringCchPrintf(szBuf, _countof(szBuf), TEXT("/select,%s"), lvi.pszText);
				ShellExecute(hWnd, TEXT("open"), TEXT("explorer"), szBuf, NULL, SW_SHOW);
				break;
			}
			case ID_PAUSE_PROCESS: {	// 暂停进程
				nSelected = SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETSELECTIONMARK, 0, 0);
				lvi.iItem = nSelected;	lvi.iSubItem = 1;	lvi.mask = LVIF_TEXT;	
				lvi.pszText = szProcessID;	lvi.cchTextMax = _countof(szProcessID);
				SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETITEM, 0, (LPARAM)&lvi);
				SuspendProcess(_ttoi(lvi.pszText), TRUE);
				break;
			}
			case ID_RESUME_PROCESS: {	// 恢复进程
				nSelected = SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETSELECTIONMARK, 0, 0);
				lvi.iItem = nSelected;	lvi.iSubItem = 1;	lvi.mask = LVIF_TEXT;	
				lvi.pszText = szProcessID;	lvi.cchTextMax = _countof(szProcessID);
				SendMessage(GetDlgItem(hWnd, IDC_LIST), LVM_GETITEM, 0, (LPARAM)&lvi);
				SuspendProcess(_ttoi(lvi.pszText), FALSE);

				break;
			}
			}
			break;
		}
		case WM_NOTIFY: {
			if (((LPNMHDR)lParam)->idFrom == IDC_LIST && ((LPNMHDR)lParam)->code == NM_RCLICK) {
				if (((LPNMITEMACTIVATE)lParam)->iItem < 0) return FALSE;
				// 如果可执行文件路径一列为空，则禁用结束该进程、打开文件所在位置、暂停进程、结束进程菜单
				nSelected = SendMessage(GetDlgItem(g_hWnd, IDC_LIST), LVM_GETSELECTIONMARK, 0, 0);
				hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU1));

				lvi.iItem = nSelected; lvi.iSubItem = 3; lvi.mask = LVIF_TEXT;
				lvi.pszText = szProcessName; lvi.cchTextMax = _countof(szProcessID);

				SendDlgItemMessage(g_hWnd, IDC_LIST, LVM_GETITEM, 0, (LPARAM)&lvi);

				if (_tcsicmp(lvi.pszText, TEXT("")) == 0) {
					EnableMenuItem(hMenu, ID_END_PROCESS, MF_BYCOMMAND | MF_DISABLED);
					EnableMenuItem(hMenu, ID_OPEN_LOCATION, MF_BYCOMMAND | MF_DISABLED);
					EnableMenuItem(hMenu, ID_PAUSE_PROCESS, MF_BYCOMMAND | MF_DISABLED);
					EnableMenuItem(hMenu, ID_RESUME_PROCESS, MF_BYCOMMAND | MF_DISABLED);
				}

				// 弹出快捷菜单
				GetCursorPos(&pt);

				TrackPopupMenu(GetSubMenu(hMenu, 0), TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hWnd, NULL);
			}
			break;
		}
	}
	return FALSE;
}

BOOL GetProcessList() {
	HANDLE hSnapshot;
	PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32)};
	BOOL bRet;
	HANDLE hProcess;
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szBuf[16] = { 0 };
	DWORD dwLen;
	SHFILEINFO fi = { 0 };
	int nImage;
	LVITEM lvi = { 0 };
	// 删除图像列表中的所有图像
	ImageList_Remove(g_hImageListSmall, -1);
	// 删除所有列表项
	SendMessage(GetDlgItem(g_hWnd, IDC_LIST), LVM_DELETEALLITEMS, 0, 0);

	// 获取系统所有进程快照
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE){
		MessageBox(g_hWnd, TEXT("CreateToolhelp32Snapshot函数调用失败"), TEXT ("提示"), MB_OK);
		return FALSE;
	}

	// 检索快照中的第一个进程信息
	bRet = Process32First(hSnapshot, &pe);
	while (bRet) {
		nImage = -1;
		ZeroMemory(szPath, sizeof(szPath));
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
		if (hProcess) {
			// 获取可执行文件的路径
			dwLen = _countof(szPath);
			QueryFullProcessImageName(hProcess, 0, szPath, &dwLen);

			// 获取程序图标
			SHGetFileInfo(szPath, 0, &fi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON);
			if (fi.hIcon) {
				nImage = ImageList_AddIcon(g_hImageListSmall, fi.hIcon);
			}

			CloseHandle(hProcess);
			lvi.mask = LVIF_IMAGE | LVIF_TEXT;	// 图标和文本有效
			lvi.iItem = SendMessage(GetDlgItem(g_hWnd, IDC_LIST), LVM_GETITEMCOUNT, 0, 0); // 设置行编号

			// 第1列，进程可执行文件名称
			lvi.iSubItem = 0 ; lvi.pszText = pe.szExeFile; lvi.iImage = nImage;
			SendMessage(GetDlgItem(g_hWnd, IDC_LIST), LVM_INSERTITEM, 0, (LPARAM)&lvi);
			if (fi.hIcon) {
				DestroyIcon(fi.hIcon);
			}

			// 第2列，进程ID
			_itow_s(pe.th32ProcessID, szBuf, _countof(szBuf), 10);	// 10进制整数转字符串
			lvi.mask = LVIF_TEXT; lvi.iSubItem = 1;	lvi.pszText = szBuf;
			SendMessage(GetDlgItem(g_hWnd,IDC_LIST), LVM_SETITEM, 0, (LPARAM)&lvi);

			// 第3列，父进程ID
			_itow_s(pe.th32ParentProcessID, szBuf, _countof(szBuf), 10);			
			lvi.mask = LVIF_TEXT; lvi.iSubItem = 2;	lvi.pszText = szBuf;
			SendMessage(GetDlgItem(g_hWnd,IDC_LIST), LVM_SETITEM, 0, (LPARAM)&lvi);

			// 第4列，可执行文件路径
			lvi.mask = LVIF_TEXT; lvi.iSubItem = 3;	lvi.pszText = szPath;
			SendMessage(GetDlgItem(g_hWnd,IDC_LIST), LVM_SETITEM, 0, (LPARAM)&lvi);

		}


		bRet = Process32Next(hSnapshot, &pe);
	}

	CloseHandle(hSnapshot);
	return FALSE;

}

VOID SuspendProcess(DWORD dwProcessId, BOOL bSuspend) {
	HANDLE hSnapshot;
	THREADENTRY32 te = { sizeof(THREADENTRY32) };
	BOOL bRet = FALSE;
	HANDLE hThread;

	// 枚举所有线程
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(g_hWnd, TEXT("CreateToolhelp32Snapshot函数调用失败"), TEXT ("提示"), MB_OK);
		return;
	}

	bRet = Thread32First(hSnapshot, &te);

	while (bRet) {
		if (te.th32OwnerProcessID == dwProcessId) {
			hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
			if (hThread) {
				if (bSuspend) {
					SuspendThread(hThread);
				}else {
					ResumeThread(hThread);
				}
				CloseHandle(hThread);
			}
		}
		bRet = Thread32Next(hSnapshot, &te);
	}

	CloseHandle(hSnapshot);
}