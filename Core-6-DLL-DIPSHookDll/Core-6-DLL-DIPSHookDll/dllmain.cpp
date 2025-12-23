// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "pch.h"
#include "resource.h"
#include <stdlib.h>
#include <Commctrl.h>
#pragma comment(lib, "Comctl32.lib")

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
    switch (uMsg) {
		case WM_APP: {
            if (lParam) {
                SaveListViewItemPositions((HWND)wParam);
            }else {
                RestoreListViewItemPositions((HWND)wParam);
            }
			break;
		}
        case WM_CLOSE: {
            DestroyWindow(hWnd);
            return TRUE;
        }
    }
    return FALSE;
}

VOID SaveListViewItemPositions(HWND hwndLV) {
    HKEY hKey;
    LVITEM li = {0};
    int nCount = 0;
    TCHAR szName[MAX_PATH] = { 0 };
    POINT pt;
    // 先删除旧注册表
    RegDeleteKey(HKEY_CURRENT_USER, g_szRegSubKey);
    // 获取桌面列表项总数
    nCount = SendMessage(hwndLV, LVM_GETITEMCOUNT, 0, 0);
    // 创建子键
    DWORD dwDisposition;
    LONG lRet;
    lRet = RegCreateKeyEx(HKEY_CURRENT_USER, g_szRegSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
    
    // 为每个列表项创建一个键值项，以列表项的文本为键名，以列表项的位置为键值
    li.mask = LVIF_TEXT;    
    li.pszText = szName;
    li.cchTextMax = _countof(szName);
    for (int i = 0; i < nCount; i++) {
        SendMessage(hwndLV, LVM_GETITEMTEXT, i, (LPARAM)&li);
        SendMessage(hwndLV, LVM_GETITEMPOSITION, i, (LPARAM)&pt);
        RegSetValueEx(hKey, li.pszText, 0, REG_BINARY, (LPBYTE)&pt, sizeof(POINT));
    }
    RegCloseKey(hKey);
}

VOID RestoreListViewItemPositions(HWND hwndLV) {
    HKEY hKey;
    TCHAR szName[MAX_PATH] = { 0 };
    POINT pt;
    DWORD dwType;
    LONG_PTR lStyle;
    LVFINDINFO lvfi = { 0 };
    int nItem;
    // 打开子键
    RegOpenKeyEx(HKEY_CURRENT_USER, g_szRegSubKey, 0, KEY_READ, &hKey);
    // 关闭桌面图标自动排列
    lStyle = GetWindowLongPtr(hwndLV, GWL_STYLE);
    if (lStyle & LVS_AUTOARRANGE)
        SetWindowLongPtr(hwndLV, GWL_STYLE, lStyle & ~LVS_AUTOARRANGE);

    //枚举子键
    LONG lResult = ERROR_SUCCESS;
    for (int i = 0; lResult != ERROR_NO_MORE_ITEMS; i++) {
        DWORD lpcchName = _countof(szName);
        DWORD dwCbData = sizeof(pt);
        lResult =RegEnumValue(hKey, i, szName, &lpcchName, 0, &dwType, (LPBYTE)&pt, &dwCbData);
        if (lResult == ERROR_NO_MORE_ITEMS)
            continue;

        // 查找桌面上具有指定列表项文本的列表项，重新设置该列表项的位置
        lvfi.flags = LVFI_STRING;
        lvfi.psz = szName;
        if ((dwType == REG_BINARY) && (dwCbData == sizeof(pt))){
            nItem = SendMessage(hwndLV, LVM_FINDITEM, -1, (LPARAM)&lvfi);
            if (nItem != -1)
                SendMessage(hwndLV, LVM_SETITEMPOSITION, nItem, MAKELPARAM(pt.x, pt.y));
        }

    }

    SetWindowLongPtr(hwndLV, GWL_STYLE, lStyle);

    RegCloseKey(hKey);
}

