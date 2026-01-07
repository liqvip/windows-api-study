#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include "resource.h"

HWND g_hWndDlg;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ThreadProc(LPVOID lpParameter);

BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
;
BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, 0);
	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HANDLE hThread;
	DWORD dwProcessId;
	TCHAR szDllPath[MAX_PATH] = { 0 };


	switch (uMsg) {
		case WM_INITDIALOG:
			g_hWndDlg = hWnd;
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_BUTTON_INJECT_DLL: {	// 注入dll
					// 创建新线程完成对目标进程中DLL的注入
					SetDlgItemText(hWnd, IDC_EDIT_MODULE_LIST, TEXT(""));
					hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
					if (hThread)
						CloseHandle(hThread);
					break;
				}
				case IDC_BUTTON_REMOVE_DLL: {	// 卸载dll
					SetDlgItemText(hWnd, IDC_EDIT_MODULE_LIST, TEXT(""));
					dwProcessId = GetDlgItemInt(hWnd, IDC_EDIT_PROCESS_ID, NULL, FALSE);
					GetDlgItemText(hWnd, IDC_EDIT_DLL_PATH, szDllPath, _countof(szDllPath));
					EjectDll(dwProcessId, szDllPath);
					break;
				}
				case IDOK:
				case IDCANCEL: {
					EndDialog(hWnd, 0);
					break;
				}
			}
			break;
	}
	return FALSE;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter){
	DWORD dwProcessId;
	TCHAR szDllPath[MAX_PATH] = { 0 };
	dwProcessId = GetDlgItemInt(g_hWndDlg, IDC_EDIT_PROCESS_ID, NULL, FALSE);
	GetDlgItemText(g_hWndDlg, IDC_EDIT_DLL_PATH, szDllPath, _countof(szDllPath));
	return InjectDll(dwProcessId, szDllPath);
}

BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath){
	HANDLE hProcess = NULL;
	LPTSTR lpDllPathRemote = NULL;
	HANDLE hThread = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);

	if (!hProcess)
		return FALSE;

	// 1. 调用VirtualAllocEx函数在远程进程的地址空间中分配一块内存
	int cbDllPath = (_tcslen(lpDllPath) + 1) * sizeof(TCHAR);
	lpDllPathRemote = (LPTSTR)VirtualAllocEx(hProcess, NULL, cbDllPath, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lpDllPathRemote)
		return FALSE;
	// 2. 调用WriteProcessMemory函数把要注入的DLL的路径复制到第1步分配的内存中
	if (!WriteProcessMemory(hProcess, lpDllPathRemote, lpDllPath, cbDllPath, NULL))
		return FALSE;

	// 3. 调用GetProcAddress函数得到LoadLibraryA / LoadLibraryW函数（Kernel32.dll）的实际地址
	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if (!pfnThreadRtn)
		return FALSE;

	// 4. 调用CreateRemoteThread函数在远程进程中创建一个线程
	hThread = CreateRemoteThread(hProcess, NULL, 0,pfnThreadRtn, lpDllPathRemote, 0, NULL);
	if (!hThread)
		return FALSE;
	WaitForSingleObject(hThread, INFINITE);

	// 5. 调用VirtualFreeEx函数释放第1步分配的内存
	if (!lpDllPathRemote)
		VirtualFreeEx(hProcess, lpDllPathRemote, 0,MEM_RELEASE);
	if (hThread)
		CloseHandle(hThread);
	if (hProcess)
		CloseHandle(hProcess);
	return TRUE;
}

BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath){
	HANDLE hSnapshot;
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	BOOL bRet;
	BOOL bFound = FALSE;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;

	bRet = Module32First(hSnapshot, &me);
	while (bRet){
		if (_tcsicmp(TEXT("Core-6-DLL-RemoteDll.dll"), me.szModule) == 0 ||_tcsicmp(lpDllPath, me.szExePath) == 0){
			bFound = TRUE;
			break;
		}
		bRet = Module32Next(hSnapshot, &me);
	}

	if (!bFound)
		return FALSE;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |PROCESS_VM_OPERATION, FALSE, dwProcessId);

	if (!hProcess)
		return FALSE;

	// 6. 调用GetProcAddress得到FreeLibrary函数（Kernel32.dll）的实际地址
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
	if (!pfnThreadRtn)
		return FALSE;

	// 7. 调用CreateRemoteThread函数在远程进程中创建一个新线程，
	// 让该线程调用FreeLibrary函数并在参数中传入已注入DLL的模块地址以卸载该DLL
	hThread = CreateRemoteThread(hProcess, NULL, 0,pfnThreadRtn, me.modBaseAddr, 0, NULL);
	if (!hThread)
		return FALSE;
	WaitForSingleObject(hThread, INFINITE);

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
	if (hThread)
		CloseHandle(hThread);
	if (hProcess)
		CloseHandle(hProcess);
	return TRUE;
}

