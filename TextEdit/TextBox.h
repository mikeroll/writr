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
	//Tfont font[MAX_LEN_TEXT];
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
	int SystemKey(WPARAM wParam);
	void MoveCar(char dir);
	void CreateCar(HWND hWnd);
	void MouseDown(LPARAM lParam);
	void MouseUp(LPARAM lParam, HWND hWnd);
	void MouseMove(LPARAM lParam, HWND hWnd);
	void SelectOrSetCaret(HWND hWnd);
	void swap(Point *a, Point *b);

};

