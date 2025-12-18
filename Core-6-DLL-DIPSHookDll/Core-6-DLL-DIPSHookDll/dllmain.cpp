// dllmain.cpp : 定义 DLL 应用程序的入口点。

#define DLL_EXPORT
#include "pch.h"
#include "resource.h"

HINSTANCE g_hMod;
HHOOK g_hHook;
TCHAR g_szRegSubKey[] = TEXT("Software\\Desktop Item Position Saver");

// 内部函数
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID SaveListViewItemPositions(HWND hwndLV);

VOID RestoreListViewItemPositions(HWND hwndLV);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hMod = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


DLL_API BOOL WINAPI InstallHook(int idHook, DWORD dwThreadId) {
    if (!g_hHook) {
        g_hHook = SetWindowsHookEx(idHook, GetMsgProc, g_hMod, dwThreadId);
        if (!g_hHook) {
            return FALSE;
        }
    }
    // 消息钩子已经安装，通知资源管理器线程调用GetMsgProc钩子函数（为了及时响应所以主动通知）
    PostThreadMessage(dwThreadId, WM_NULL, 0, 0);
    return TRUE;
}

DLL_API BOOL WINAPI UnInstallHook() {
    if (g_hHook){
        if (!UnhookWindowsHookEx(g_hHook))
            return FALSE;
    }
    g_hHook = NULL;
    return TRUE;
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static BOOL bFirst = TRUE;
    if (nCode < 0)
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    if (nCode == HC_ACTION){
        if (bFirst){
            bFirst = FALSE;
            // 在资源管理器进程中创建一个服务器窗口来处理控制程序的请求（保存、恢复桌面图标等）
            CreateDialogParam(g_hMod, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);

}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

}

