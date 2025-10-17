#include <Windows.h>
#include <strsafe.h>

#pragma comment(lib, "Winmm.lib")   // 播放声音的PlaySound函数需要Winmm导入库

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                        // RegisterClassEx函数用的WNDCLASSEX结构
    TCHAR szClassName[] = TEXT("MyWindow");     // RegisterClassEx函数注册的窗口类的名称
    TCHAR szAppName[] = TEXT("HelloWindows");   // 窗口标题
    HWND hwnd;                                  // CreateWindowEx函数创建的窗口的句柄
    MSG msg;                                    // 消息循环所用的消息结构体

    wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 窗口类风格
	wndclass.lpfnWndProc = WindowProc; // 指向窗口过程的指针
	wndclass.cbClsExtra = 0; // 额外的类内存
	wndclass.cbWndExtra = 0; // 额外的窗口内存
	wndclass.hInstance = hInstance; // 该应用程序实例的句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 默认图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // 默认箭头光标
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 白色背景画刷
	wndclass.lpszMenuName = NULL; // 没有菜单
	wndclass.lpszClassName = szClassName; // 窗口类名
	wndclass.hIconSm = NULL; // 小图标
	RegisterClassEx(&wndclass); // 注册窗口类

	// 获取屏幕宽高
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowWidth = 800;
	int windowHeight = 800;
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

    hwnd = CreateWindowEx(
		0, // 扩展风格，通常为0
		szClassName, // 窗口类名
		szAppName, // 窗口标题
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // 窗口风格
		x, // x坐标
		y, // y坐标
		windowWidth, // 宽度
		windowHeight, // 高度
		NULL, // 父窗口句柄
		NULL, // 菜单句柄
		hInstance, // 该应用程序实例的句柄
		NULL
    );

	ShowWindow(hwnd, nCmdShow); // 显示窗口
	UpdateWindow(hwnd); // 更新窗口

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	// HDC : 设备上下文句柄，用于绘图
    HDC hdc;
	// PAINTSTRUCT : 绘图结构体，用于BeginPaint和EndPaint函数
    PAINTSTRUCT ps;
    TCHAR szStr[] = TEXT("你好，Windows程序设计");

    switch (uMsg){
        case WM_CREATE:
            PlaySound(TEXT("成都(两会版).wav"), NULL, SND_FILENAME | SND_ASYNC/* | SND_LOOP*/);
            return 0;
        case WM_PAINT:
		    hdc = BeginPaint(hwnd, &ps);
            TextOut(hdc, 50, 50, szStr, lstrlen(szStr));
            EndPaint(hwnd, &ps);
		    return 0;
		case WM_LBUTTONDBLCLK:
			// 提示双击
			MessageBox(hwnd, TEXT("你双击了鼠标左键"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);

			return 0;
		case WM_CHAR: {
			// 提示你按下了哪个键
			TCHAR szBuff[16] = { 0 };
			// _countof(szBuff) 获取数组元素个数
			// sizeof(szBuff) 获取数组字节数
			StringCchPrintf(szBuff, _countof(szBuff), TEXT("你按下了 %c 键, %d"), wParam, sizeof(szBuff));
			MessageBox(hwnd, szBuff, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			return 0;
		}
		case WM_CLOSE:
			// 提示是否关闭窗口
            if (MessageBox(hwnd, TEXT("真的要关闭窗口吗？"), TEXT("关闭窗口"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
				// 关闭窗口
                DestroyWindow(hwnd);
            }
			return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
		default:
			// 其他消息交给默认的窗口过程处理
			// DefWindowProc 不会处理WM_DESTROY消息
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}