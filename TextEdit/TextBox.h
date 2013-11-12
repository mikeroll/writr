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

    static const UINT MAX_LENGTH=2000;
    TCHAR text[MAX_LENGTH];
    BYTE font[MAX_LENGTH];
    UINT length;
    //TODO: Timg image[MAX_LENGTH];


    // Caret control

    UINT caretPos;
    LONG maxLineHeight;
    enum Direction { LEFT, RIGHT, UP, DOWN };


    // Selection control

    BOOL isClicked;
    BOOL isDblClicked;
    BOOL isSelected;
    UINT selectStart = 0, selectEnd = 0;
    Point MStart, MEnd;     //MouseDown -> Mstart, MouseUp ->Mend


    // Color

	COLORREF SelectColor = RGB(78, 221, 110);
	//COLORREF BackColor = RGB(67, 78, 84);
	COLORREF TextColor = RGB(220, 220, 220);
	COLORREF SelectTextColor = RGB(30, 30, 30);


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

    inline UINT     GetLength()  { return length; }
    inline UINT     SetLength(UINT n) { length = n; text[n] = (TCHAR)0; return length; }
    inline LPTSTR   GetTextPtr() { return text; }
    inline PBYTE    GetFontPtr() { return font; }
};

