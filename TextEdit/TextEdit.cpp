// TextEdit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <string.h>
#include <windowsx.h>

#include "TextEdit.h"

#include "TextBox.h"
#include "WritrDocument.h"


#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// App data structure
struct AppState {
    TextBox *editor;
    //HistoryCtl *history;
};

// Routine for retreiving state
inline AppState* GetAppState(HWND hWnd)
{
    LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    AppState *pState = reinterpret_cast<AppState *>(ptr);
    return pState;
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, AppState *);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void    CreatePopup(HWND, LPARAM);



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TEXTEDIT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Initialize app data
    AppState *pState = new AppState();
    pState->editor = new TextBox();

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow, pState))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEXTEDIT));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
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
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTEDIT));
    wcex.hCursor = LoadCursor(NULL, IDC_IBEAM);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEXTEDIT);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, AppState *pState)
{
    // Initialize data struct

    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowEx(
        0,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        NULL,
        NULL,
        hInstance,
        pState
        );

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    

    AppState *pState;

    if (message == WM_NCCREATE)
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pState = reinterpret_cast<AppState *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pState);
    }
    else
    {
        pState = GetAppState(hWnd);
    }

    TextBox *editor;
    if (pState != NULL)
    {
        editor = pState->editor;
    }
    else
        return -1;

    switch (message)
    {
    case WM_LBUTTONDBLCLK:
        editor->SelectWord(hWnd);
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_LBUTTONDOWN:
        editor->MouseDown(lParam);
        editor->ReDrawBox(hWnd);
        break;
    case WM_LBUTTONUP:
        editor->MouseUp(lParam,hWnd);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK)
            editor->Removing(hWnd,wParam);
        else if (!(GetKeyState(VK_CONTROL) < 0))
            editor->KeyPress(hWnd,wParam);
        //InvalidateRect(hWnd,NULL,TRUE);//чтобы вызвать WM_PAINT
        break;
    case WM_KEYDOWN:
        if (wParam == VK_DELETE)
        {   editor->Removing(hWnd,wParam);  }
        else if (editor->SystemKey(wParam,hWnd))
            editor->ReDrawBox(hWnd);
        break;
    case WM_CONTEXTMENU:
        CreatePopup(hWnd,lParam);
        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_EDIT_INSERTIMAGE:
            //editor->InsertImage(hWnd);
            break;
        case ID_EDIT_ARIAL:
            if (editor->SetCurrentFont(0))
                editor->ReDrawBox(hWnd);
            break;
        case ID_EDIT_TIMESNEWROMAN:
            if (editor->SetCurrentFont(1))
                editor->ReDrawBox(hWnd);
            break;
        case ID_EDIT_KRISTENITC:
            if (editor->SetCurrentFont(2))
                editor->ReDrawBox(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        editor->ReDrawBox(hWnd);
        EndPaint(hWnd, &ps);
        break;
    case WM_SIZE:
        editor->ResizeBox(lParam);
        break;
    case WM_MOUSEWHEEL:
        if (GetKeyState(VK_CONTROL) < 0)
        {
            if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
                editor->ZoomIn();
            else
                editor->ZoomOut();
            editor->ReDrawBox(hWnd);
        }		
        break;
    case WM_DESTROY:
        delete editor;
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

void CreatePopup(HWND hWnd, LPARAM lParam)
{
    //cursor's coordinates
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    //if call from keyboard
    if (pt.x == -1 && pt.y == -1)
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        pt.x = rect.left + 5;
        pt.y = rect.top + 5;
    }

    //load menu from resourses
    HMENU hMenu, hPopupMenu;
    hMenu = (LoadMenu(hInst, MAKEINTRESOURCE(IDC_TEXTEDIT)));
    hPopupMenu = GetSubMenu(hMenu, 1);

    //show menu
    TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}