#pragma once
class TextBox
{
private:

	struct Point
	{
		int x;
		int y;
	};
	//------------------Main block---------
	static const int MAX_LEN_TEXT=2000;
	char text[MAX_LEN_TEXT];
	//Timg image[MAX_LEN_TEXT];
	BYTE font[MAX_LEN_TEXT];
	int CaretPos;
	int CountElements;
	int MaxHght;		//max line height

	//------------------Client Rect--------------
	int WALL;		//right edge of the window
	int GROUND;		//bottom edge of the window
	RECT r;	

	//------------------Color-------------------
	COLORREF SelectColor = RGB(78, 221, 110);
	COLORREF BackColor = RGB(67, 78, 84);
	COLORREF TextColor = RGB(200, 200, 200);
	COLORREF SelectTextColor = RGB(30, 30, 30);

	//----------------Fonts-------------------
	LOGFONTW Font[3]; /*= { { -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 33, _T("@Arial Unicode MS") },
						{ -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 18, _T("Times New Roman") }, 
						{ -16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 66, _T("Kristen ITC") } };*/
	BYTE CurrentFont;
	LONG zoom;
	//-----------------Flags------------------
	bool Click;
	bool DBLClicK;
	bool Select;
	int SelectStart=0, SelectEnd=0;

	Point MStart, MEnd;	//MouseDown -> start, MouseUp ->end


public:
	TextBox();
	~TextBox();
	void AddText(char ch);
	void RemoveText();
	void SetBox(LPARAM lParam);
	void ReDrawBox(HWND hWnd);
	void KeyPress(HWND hWnd, WPARAM wParam);
	int SystemKey(WPARAM wParam,HWND hWnd);
	void MoveCar(char dir,HWND hWnd);
	void CreateCar(HWND hWnd, int height);
	void MouseDown(LPARAM lParam);
	void MouseUp(LPARAM lParam, HWND hWnd);
	void MouseMove(LPARAM lParam, HWND hWnd);
	void SelectOrSetCaret(HWND hWnd);
	void swap(Point *a, Point *b);
	void WheelUP();
	void WheelDN();
	void SetCurrentFont(BYTE f);

};

