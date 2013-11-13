#pragma once
class TextBox
{
private:

    struct Point
    {
        int x;
        int y;
    };
    
    // Client Rect
    
    WORD wall;      //right edge of the window
    WORD ground;    //bottom edge of the window
    RECT r;

    //Text and formatting

    static const int MAX_LENGTH = 2000;
    TCHAR text[MAX_LENGTH];
    BYTE font[MAX_LENGTH];
    int length;
    //TODO: Timg image[MAX_LENGTH];


    // Caret control
    
    int caretPos;
    LONG maxLineHeight;
    enum Direction { LEFT, RIGHT, UP, DOWN };


    // Selection control
    
    BOOL isClicked;
    BOOL isDblClicked;
    BOOL isSelected;
    int selectStart, selectEnd;
    Point MStart, MEnd;     //MouseDown -> Mstart, MouseUp ->Mend 


    // Color
    
    COLORREF SelectColor = RGB(78, 221, 110);
    COLORREF TextColor = COLOR_WINDOWTEXT;
    COLORREF SelectTextColor = RGB(255, 255, 255);


    // Fonts
    
    LOGFONT Font[3];
    BYTE CurrentFont;
    LONG zoom;
  
    
public:
    TextBox();
    ~TextBox();
    VOID InsertChar(char ch);
    VOID RemoveChar();

    VOID ResizeBox(LPARAM lParam);
    VOID ReDrawBox(HWND hWnd);
    VOID KeyPress(HWND hWnd, WPARAM wParam);
    BOOL SystemKey(WPARAM wParam,HWND hWnd);
    VOID MoveCar(Direction dir,HWND hWnd);
    VOID CreateCar(HWND hWnd, int height);
    VOID MouseDown(LPARAM lParam);
    VOID MouseUp(LPARAM lParam, HWND hWnd);
    VOID MouseMove(LPARAM lParam, HWND hWnd);
    VOID SelectOrSetCaret(HWND hWnd);
    VOID swap(Point *a, Point *b);
    VOID ZoomIn();
    VOID ZoomOut();
    BOOL SetCurrentFont(BYTE f);
    VOID SelectWord(HWND hWnd);
    BOOL IsNormalChar(TCHAR ch);
    VOID InsertImage(HWND hWnd);
    VOID Removing(HWND hWnd, WPARAM wParam);

    inline UINT     GetLength()  { return length; }
    inline UINT     SetLength(UINT n) { length = n; text[n] = (TCHAR)0; return length; }
    inline LPTSTR   GetTextPtr() { return text; }
    inline PBYTE    GetFontPtr() { return font; }
};

