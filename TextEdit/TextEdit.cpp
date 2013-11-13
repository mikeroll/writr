// TextEdit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <string.h>
#include <commdlg.h>
#include <windowsx.h>

#include "TextEdit.h"

#include "TextBox.h"
#include "HistoryCtl.h"
#include "WritrDocument.h"


#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::wstring baseDocName(L"untitled", 8);

// App data structure
struct AppState {
    TextBox *editor;
    HistoryCtl *history;
    WritrDocument *document;
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
BOOL                InitInstance(HINSTANCE, int);
//BOOL                InitInstance(HINSTANCE, int, AppState *);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


enum FileAction { FA_OPEN, FA_SAVE, FA_ADDIMAGE };
std::wstring        ChooseFile(FileAction action);

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

    
    // Perform application initialization:
    //if (!InitInstance (hInstance, nCmdShow, pState))
    if (!InitInstance (hInstance, nCmdShow))
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
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, AppState *pState)
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // Initialize data struct

    HWND hWnd;

    // Initialize app data
    AppState *pState = new AppState();
    
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
        pState->editor = new TextBox(hWnd);
        pState->history = new HistoryCtl(pState->editor, 5);
        pState->document = new WritrDocument(baseDocName, pState->editor);
    }
    else
    {
        pState = GetAppState(hWnd);
    }

    TextBox *editor;
    HistoryCtl *history;
    WritrDocument *document;
    if (pState != NULL)
    {
        editor = pState->editor;
        history = pState->history;
        document = pState->document;
    }
    else
        return -1;

    switch (message)
    {
    case WM_LBUTTONDBLCLK:
        editor->SelectWord();
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_LBUTTONDOWN:
        if(editor->MouseDown(lParam))
            editor->ReDrawBox();
        break;
    case WM_LBUTTONUP:
        editor->MouseUp(lParam);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK)
            editor->Removing(wParam);
        else if (!(GetKeyState(VK_CONTROL) < 0)) {
            editor->KeyPress(wParam);
        }
        history->Memorize();
        //InvalidateRect(hWnd,NULL,TRUE);//чтобы вызвать WM_PAINT
        break;
    case WM_KEYDOWN:
        if (wParam == VK_DELETE)
        {   
            editor->Removing(wParam);
            history->Memorize();
        }
        else if (editor->SystemKey(wParam))
            editor->ReDrawBox();
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

        // File
        case ID_FILE_NEW:
            document->New();
            break;
        case ID_FILE_SAVE:
            document->SetName(ChooseFile(FA_SAVE));
            document->Save();
            break;
        case ID_FILE_OPEN:
            document->SetName(ChooseFile(FA_OPEN));
            document->Load();
            break;

        // Edit
        case ID_EDIT_UNDO:
            history->Undo();
            editor->ReDrawBox();
            break;
        case ID_EDIT_REDO:
            history->Redo();
            editor->ReDrawBox();
            break;
        case ID_EDIT_CUT:
            break;
        case ID_EDIT_COPY:
            break;
        case ID_EDIT_PASTE:
            break;

        // Insert
        case ID_INSERT_IMAGE:
            break;

        // Fonts
        case ID_FONT_ARIAL:
            if (editor->SetCurrentFont(0)) {
                history->Memorize();
                editor->ReDrawBox();
            }
            break;
        case ID_FONT_TIMESNEWROMAN:
            if (editor->SetCurrentFont(1)) {
                history->Memorize();
                editor->ReDrawBox();
            }
            break;
        case ID_FONT_KRISTENITC:
            if (editor->SetCurrentFont(2)) {
                history->Memorize();
                editor->ReDrawBox();
            }
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        editor->ReDrawBox();
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
            editor->ReDrawBox();
        }		
        break;
    case WM_DESTROY:
        delete editor;
        delete history;
        delete document;
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

std::wstring ChooseFile(FileAction action)
{
    OPENFILENAME ofn = { 0 };
    const DWORD maxFilename = 512;
    TCHAR filename[maxFilename];
    filename[0] = (TCHAR)0;
    TCHAR *filter;

    if (action == FA_ADDIMAGE)
        filter = L"Bitmap image\0*.bmp\0\0";
    else
        filter = L"Writr document\0*.wdoc\0\0";

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = maxFilename;
    ofn.lpstrFilter = filter;

    if (action == FA_SAVE)
        GetSaveFileName(&ofn);
    else
        GetOpenFileName(&ofn);

    std::wstring wFilename = filename;

    return wFilename;
}