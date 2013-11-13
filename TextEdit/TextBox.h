#pragma once
#include <string>
#include "ImageList.h"

#define MAX_LENGTH 2000

struct EditorState
{
    TCHAR text[MAX_LENGTH];
    BYTE font[MAX_LENGTH];
    INT length;
    INT caretPos;
};

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

    TCHAR text[MAX_LENGTH];
    BYTE font[MAX_LENGTH];
    int length;
    //TODO: std::list<Timg> images;


    // Caret control

    int caretPos;
    LONG maxLineHeight;
    enum Direction { LEFT, RIGHT, UP, DOWN };


    // Selection control

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


    // Window

    HWND hWnd;


    // Image

    int imgCount;
    ImageList *images;

    
public:
    BOOL isClicked;
    TextBox(HWND hwnd);
	~TextBox();
    VOID InsertChar(char ch);
    VOID RemoveChar();

    VOID ResizeBox(LPARAM lParam);
    VOID ReDrawBox();
    VOID KeyPress(WPARAM wParam);
    BOOL SystemKey(WPARAM wParam);
    VOID MoveCar(Direction dir);
    VOID CreateCar(int height);
    BOOL MouseDown(LPARAM lParam);
    VOID MouseUp(LPARAM lParam);
    VOID MouseMove(LPARAM lParam);
    VOID SelectOrSetCaret();
    VOID swap(Point *a, Point *b);
    VOID ZoomIn();
    VOID ZoomOut();
    BOOL SetCurrentFont(BYTE f);
    VOID SelectWord();
    BOOL IsNormalChar(TCHAR ch);
    VOID Removing(WPARAM wParam);
    std::string GetSelection();
    VOID InsertString(std::string);

    VOID InsertImage();

    EditorState GetState();
    VOID LoadState(EditorState state);
};

