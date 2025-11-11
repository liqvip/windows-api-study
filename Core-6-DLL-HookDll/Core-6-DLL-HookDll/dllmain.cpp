// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <stdlib.h>

// 全局变量
HINSTANCE g_hInstance = NULL; // DLL 模块句柄
HHOOK g_hHook = NULL;       // 钩子句柄
TCHAR g_szBuffer[256] = { 0 }; // 用于存储键盘输入的缓冲区

#pragma data_seg("Shared")
    HWND g_hWnd = NULL;         // 用于发送消息的窗口句柄
#pragma data_seg()

#pragma comment(linker, "/SECTION:Shared,RWS")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH: // DLL 映射到进程地址空间时调用
		g_hInstance = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: // DLL 从进程地址空间卸载时调用
        break;
    }
    return TRUE;
}

BOOL WINAPI InstallHook(int idHook, DWORD dwThreadId, HWND hwnd)
{
    // 安装钩子
    if (!g_hHook) {
		// 钩子函数类型, 钩子函数指针, DLL 实例句柄, 线程 ID
        g_hHook = SetWindowsHookEx(idHook, KeyboardProc, g_hInstance, dwThreadId);
		g_hWnd = hwnd;
        return (g_hHook != NULL);
    }
	return FALSE;
}

BOOL WINAPI UnInstallHook()
{
    // 卸载钩子
    if (g_hHook) {
        BOOL bResult = UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
        return bResult;
    }
    return FALSE;
}

// WH_KEYBOARD
// 如果nCode参数小于0，则钩子函数必须将消息传递给CallNextHookEx函数并返回CallNextHookEx函数的返回值，这种情况下不需要做其他处理
// 
// 如果nCode参数为HC_ACTION(0)，则说明wParam和lParam参数包含有关击键消息的信息，这时候我们应该对击键消息进行处理，
// 处理完后，应该调用CallNextHookEx函数将击
// 键消息传递给钩子链中的下一个钩子函数，当然，钩子函数也
// 可以通过返回TRUE来丢弃消息并阻止该消息的继续传递。

// wParam， 虚拟键代码

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BYTE bKeyState[256]; // 键盘状态数组
    COPYDATASTRUCT cds = {0};

    if (nCode < 0) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (nCode == HC_ACTION) {
		GetKeyboardState(bKeyState);
		// GetKeyState
		// @return  返回值的高位字节表示按键状态
        // 若高位字节为1，表示按键被按下，否则表示按键未被按下
		// 低位字节表示切换状态（如大写锁定键）,若低位字节为1，表示切换打开，否则表示切换关闭

        // GetKeyboardState函数获取到的键盘状态数组填充的是以 VK_LSHIFT、VK_RSHIFT、VK_LCONTROL、VK_RCONTROL为索引的数组元素，
        // 而ToUnicode函数检测的是以 VK_SHIFT、VK_CONTROL为索引的数组元素，这些按键
        // 是否按下会影响转换结果，比如同样是按键“1”，Shift键没有按下对应的就是“1”，按下的话就是“！”。在本例中我们通过
        //  GetKeyState函数获取Shift按键的状态然后赋值给 bKeyState[VK_SHIFT]。
        bKeyState[VK_SHIFT] = HIBYTE(GetKeyState(VK_SHIFT));
		// 将虚拟键代码转换为 unicode 字符
        ToUnicode(wParam,         // 虚拟键代码
                    lParam >> 16, // 扫描码
                    bKeyState,             // 键盘状态数组
                    g_szBuffer,           // 存储转换结果的缓冲区
                    _countof(g_szBuffer), // 缓冲区大小
			        0);                    // 其他参数，通常为0

        cds.cbData = sizeof(g_szBuffer);
		cds.lpData = g_szBuffer;
		SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&cds);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

