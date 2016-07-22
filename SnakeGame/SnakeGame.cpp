// SnakeGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SnakeGame.h"


#define MAX_LOADSTRING 20
//和游戏相关的全局变量
#define TIMER_ID		1

GameControl gameCtrl;
// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//	函数:SetClientSize()
//
//	目的:设置客户区大小
//
void SetClientSize(HWND hWnd, int width, int height)
{
	if (!hWnd)
	{
		//MessageBox(NULL, "窗口设置错误", NULL, 1);
		exit(1);
	}
	RECT  rectProgram, rectClient;
	GetWindowRect(hWnd, &rectProgram);   //获得程序窗口位于屏幕坐标
	GetClientRect(hWnd, &rectClient);    //获得客户区坐标
										 //非客户区宽,高
	int nWidth = rectProgram.right - rectProgram.left - (rectClient.right - rectClient.left);
	int nHeight = rectProgram.bottom - rectProgram.top - (rectClient.bottom - rectClient.top);
	nWidth += width;
	nHeight += height;

	int showToScreenx = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2;    //居中处理
	int showToScreeny = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2;
	MoveWindow(hWnd, showToScreenx, showToScreeny, nWidth, nHeight, FALSE);
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_OVERLAPPED,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetClientSize(hWnd,
	   2 * OFFSET + 2 * FRAME_LINE_WIDTH + CAP_PIXELS + LCD_COLS*(LCDDOT_PIXELS + CAP_PIXELS),
	   2 * OFFSET + 2 * FRAME_LINE_WIDTH + CAP_PIXELS + LCD_ROWS*(LCDDOT_PIXELS + CAP_PIXELS));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
VOID ClearGameArea(PVOID pvoid) {
	//gameCtrl.EraseFood();
	//gameCtrl.EraseSnake();//.ClearGameAreaRowByRow(NULL);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int       cxClient, cyClient;

    switch (message)
    {
	case WM_CREATE:
		gameCtrl.GetHWND(hWnd);
		gameCtrl.PrepareStart();
		return 0;
	case WM_SIZE:

		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_TIMER:
		if (gameCtrl.curStatus == GS_Running)
		{
			gameCtrl.Running();
		}
		else if (gameCtrl.curStatus == GS_GameOver) {
			Beep(50, 200);
			KillTimer(hWnd, TIMER_ID);
		}
		return 0;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			SetBkColor(hdc, gameCtrl.graphicsMgr.crBackground);
			//SetBkMode(hdc, TRANSPARENT);
			SetBkMode(hdc, OPAQUE);
			SetMapMode(hdc, MM_TEXT);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			//graphicsMgr.DrawGameFrame();
			gameCtrl.graphicsMgr.DrawLCDGameFrame();
			gameCtrl.DisplaySnake();
			gameCtrl.DisplayFood();
			//graphicsMgr.DrawGameArea();
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, TIMER_ID);
		PostQuitMessage(0);
        break;
	case WM_KEYDOWN:
		switch (wParam) {
			//gameOver
		case VK_F2:
			if (gameCtrl.curStatus == GS_GameOver)
			{
				//_beginthread(ClearGameArea, 0, NULL);
				gameCtrl.PrepareStart();
			}
			break;
		case VK_UP:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.userComOrient = Up;
			}
			else {
				gameCtrl.SpeedUp();
			}
			break;
		case VK_LEFT:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.userComOrient = Left;
			}
			else {
				gameCtrl.SpeedDown();
			}
			break;
		case VK_RIGHT:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.userComOrient = Right;
			}
			else {
				gameCtrl.SpeedUp();
			}
			break;
		case VK_DOWN:
			if (gameCtrl.curStatus == GS_Running)
			{
				gameCtrl.userComOrient = Down;
			}
			else {
				gameCtrl.SpeedDown();
			}
			break;
		case VK_RETURN://回车键
			if (gameCtrl.curStatus == GS_Running) {//暂停
				gameCtrl.Pause();
				KillTimer(hWnd, TIMER_ID);
			}
			else if (gameCtrl.curStatus == GS_Paused) {
				gameCtrl.Resume();
				SetTimer(hWnd, TIMER_ID, (10 - gameCtrl.level) * 50, NULL);
			}
			else if (gameCtrl.curStatus == GS_PrepareForStart)
			{
				gameCtrl.Start();
				SetTimer(hWnd, TIMER_ID, (10 - gameCtrl.level) * 50, NULL);
			}
			else if (gameCtrl.curStatus == GS_GameOver) {
			}
			break;
		case VK_SPACE:
			break;
		}
		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
