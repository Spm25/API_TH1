// API_TH1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "API_TH1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APITH1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APITH1));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APITH1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static WCHAR time[22];
    static COLORREF color = RGB(0,0,0);
    static int mau = 0, hinh = 0;
    static int wWidth, wHight;
    static POINT previousPoint;
    static int m=0, s=0;
    HDC hdc;
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_TIMER:
        s++; // Tăng giá trị của s lên 1 mỗi lần sự kiện WM_TIMER được gọi
        if (s == 60) { // Nếu s đã đạt 60, đặt lại s về 0 và tăng m lên 1
            m++;
            s = 0;
        }
        wsprintf(time, L"Thời gian là  %d:%d  ", m, s);
        hdc = GetDC(hWnd);
        TextOut(hdc, wWidth-120, wHight-15, time, wcslen(time));
        ReleaseDC(hWnd, hdc);
        break;
    case WM_SIZE:
        wWidth = LOWORD(lParam);
        wHight = HIWORD(lParam);
        break;
    case WM_RBUTTONDOWN:
        static HMENU hMenu;
        POINT point;

        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU2));
        hMenu = GetSubMenu(hMenu, 0);

        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        ClientToScreen(hWnd, &point);

        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
        break;
    case WM_LBUTTONDOWN:
    {
        previousPoint.x = LOWORD(lParam);
        previousPoint.y = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (mau == 1) color = RGB(255, 0, 0);
        else if(mau==2)color = RGB(0, 0, 255);
        if (hinh == 2) {
            POINT point;
            HDC hdc = GetDC(hWnd);
            GetCursorPos(&point);
            ScreenToClient(hWnd, &point);
            HPEN hPen = CreatePen(PS_SOLID, 3, color); // Tạo bút với màu đã chọn
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, previousPoint.x, previousPoint.y, NULL); // Di chuyển đến điểm trước đó
            LineTo(hdc, point.x, point.y); // Vẽ đường thẳng đến điểm hiện tại
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            ReleaseDC(hWnd, hdc);
        }
        else if (hinh == 1) {
            POINT point;
            HDC hdc = GetDC(hWnd);
            GetCursorPos(&point);
            ScreenToClient(hWnd, &point);
            HPEN hPen = CreatePen(PS_SOLID, 3, color); // Tạo bút với màu đã chọn
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, previousPoint.x, previousPoint.y, NULL); 
            LineTo(hdc, point.x, point.y); 
            MoveToEx(hdc, previousPoint.x, previousPoint.y, NULL); 
            LineTo(hdc, previousPoint.x, point.y); 
            MoveToEx(hdc, previousPoint.x, point.y, NULL); 
            LineTo(hdc, point.x, point.y); 
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            ReleaseDC(hWnd, hdc);
        }
        
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_MAU_DO:
                mau = 1;
                MessageBox(hWnd, L"Đỏ", L"màu", MB_OK);
                break;
            case ID_MAU_XANH:
                mau = 2;
                MessageBox(hWnd, L"Xanh", L"màu", MB_OK);
                break;
            case ID_HINH_TGV:
                hinh = 1;
                MessageBox(hWnd, L"TGV", L"HÌNH", MB_OK);
                break;
            case ID_HINH_DT:
                hinh = 2;
                MessageBox(hWnd, L"DT", L"HÌNH", MB_OK);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CLOSE:
        if (MessageBox(hWnd, L"Thoát không bạn", L"Thoát nhá", MB_OKCANCEL) == IDOK) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
