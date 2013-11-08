#include "stdafx.h"
#include "TextBox.h" 


TextBox::TextBox()
{
	WALL = 0;
	GROUND = 0;
	MaxHght = 0;
	CaretPos = 0;
	text[0] = '\0';
	CountElements = 0;
	DBLClicK = false;
	Select = false;
	Click = false;
	MStart = {0,0};
	MEnd = {0,0};
}


TextBox::~TextBox()
{
}

void TextBox::AddText(char ch)	//add one symbol
{
	if (CountElements < (MAX_LEN_TEXT-1))
	{
		for (int i = CountElements; i >= CaretPos; i--)
		{
			text[i + 1] = text[i];
		}
		text[CaretPos] = ch;
		CountElements++;
		if (CaretPos<CountElements)
			CaretPos++;
	}
}

void TextBox::RemoveText()		// Delete (not Backspace) one symbol
{
	for (int i = CaretPos; i<CountElements; i++)
	{
		text[i] = text[i+1];
	}
	text[CountElements - 1] = ' ';
	CountElements--;
	
}

void TextBox::SetBox(LPARAM lParam)
{
	WALL = LOWORD(lParam);
	GROUND = HIWORD(lParam);
}

void TextBox::ReDrawBox(HWND hWnd)
{
	Point Curr = {0,0};
	MaxHght = 0;
	SIZE s;
	char CH[2];
	CH[1] = '\0';
	r = {0,0,WALL,GROUND};
	HDC hdc = GetDC(hWnd);
	HideCaret(hWnd);
	FillRect(hdc, &r, (HBRUSH)(COLOR_3DHILIGHT));
	SetBkColor(hdc,BackColor);
	SetTextColor(hdc,TextColor);
	
	for (int i = 0; i < CountElements; i++)
	{
		DestroyCaret();
		CreateCar(hWnd);
		if (text[i] != '\r' && text[i] != '@')		//+ font, +image
		{
			if (text[i] == '_')
			{
				CH[0] = ' ';
			}
			else 
				CH[0] = text[i];
			GetTextExtentPoint(hdc, (LPCWSTR)CH, 1, &s);		//get i-char size
			if (text[i] == '_')
				s.cx *= 8;
			if (MaxHght < s.cy)
				MaxHght=s.cy;

			if ((Curr.x+s.cx) < WALL)		//if within the window, then print, else -> \r
			{
				if (CaretPos==i)		//Set Caret
				{	SetCaretPos(Curr.x, Curr.y);  }

				TextOutA(hdc, Curr.x,Curr.y,(LPCSTR)CH,1);
				Curr.x += s.cx;
			}
			else
			{
				Curr.y += MaxHght;
				MaxHght = 0;		//new line -> counter=0
				Curr.x = 0;

				if (CaretPos == i)		//Set Caret
				{	SetCaretPos(Curr.x, Curr.y);	}

				TextOutA(hdc, Curr.x, Curr.y, (LPCSTR)CH, 1);
				Curr.x += s.cx;
			}
		}
		else		//if '\r'
		{
			if (CaretPos == i)		//Set Caret
			{
				SetCaretPos(Curr.x, Curr.y);
			}

			if (MaxHght == 0)
			{
				GetTextExtentPoint(hdc, (LPCWSTR)"a", 1, &s);
				MaxHght = s.cy;
			}
			Curr.y += MaxHght;
			MaxHght = 0;		//new line -> counter=0
			Curr.x = 0;
		}
	}

	if (CaretPos == CountElements)
	{
		if (Curr.x + 2 < WALL)
			SetCaretPos(Curr.x,Curr.y);
		else
		{
			if (MaxHght!=0)
				SetCaretPos(0,Curr.y+MaxHght);
			else
			{
				GetTextExtentPoint(hdc, (LPCWSTR)"A", 1, &s);
				SetCaretPos(0, Curr.y + s.cy);
			}
		}
	}
	ShowCaret(hWnd);
	ReleaseDC(hWnd, hdc);

}

void TextBox::KeyPress(HWND hWnd, WPARAM wParam)
{
	unsigned int key = wParam;
	switch (key)
	{
	case VK_BACK:
		if (CaretPos != 0)
		{
			CaretPos--;
			RemoveText();
		}
		break;

	case VK_TAB:
		AddText('_');
		break;
	default:
		AddText(key);
		break;
	}
	ReDrawBox(hWnd);
	//+ history
}

int TextBox::SystemKey(WPARAM wParam)
{
	int result = 1;		//for ReDraw'ing window
	switch (wParam)
	{
	case VK_LEFT:
		MoveCar('l');
		break;
	case VK_RIGHT:
		MoveCar('r');
		break;
	case VK_DELETE:
		if (!Select)
			RemoveText();
		else
		{
			CaretPos = SelectStart;
			for (int i = 0; i < SelectEnd-SelectStart; i++)
			{
				RemoveText();
			}
			Select = false;
			SelectStart = 0;
			SelectEnd = 0;
		}
		break;
	default:
		result = 0;
	}
	return result;
}

void TextBox::MoveCar(char dir)
{
	switch (dir)
	{
	case 'l':
		if (CaretPos != 0)
		{
			CaretPos--;
		}
		break;
	case 'r':
		if (CaretPos < CountElements)
		{
			CaretPos++;
		}			
		break;
	case 'u':
		break;
	case 'd':
		break;
	}
}

void TextBox::CreateCar(HWND hWnd)
{
	SIZE s;
	HDC hdc = GetDC(hWnd);
	GetTextExtentPoint(hdc, (LPCWSTR)"A", 1, &s);
	ReleaseDC(hWnd, hdc);
	CreateCaret(hWnd, NULL, 2, s.cy-2);	
}

void TextBox::MouseDown(LPARAM lParam)
{
	MStart.x = LOWORD(lParam);
	MStart.y = HIWORD(lParam);
	Click = true;
	Select = false;
	SelectStart = 0;
	SelectEnd = 0;
}

void TextBox::MouseUp(LPARAM lParam,HWND hWnd)
{
	Click = false;
	MEnd.x = LOWORD(lParam);
	MEnd.y = HIWORD(lParam);
	if (MStart.x != MEnd.x || MStart.y != MEnd.y)
	{
		Select = true;		
	}
	SelectOrSetCaret(hWnd);
}

void TextBox::MouseMove(LPARAM lParam, HWND hWnd)
{
	if (Click)
	{
		MEnd.x = LOWORD(lParam);
		MEnd.y = HIWORD(lParam);
		if (MStart.x != MEnd.x && MStart.y != MEnd.y)
		{
			Select = true;
		}
		SelectOrSetCaret(hWnd);
	}
}

void TextBox::SelectOrSetCaret(HWND hWnd)		//Difference with ReDrawBox(): all text not redraw - only selected part
{
	Point Curr = { 0, 0 };
	MaxHght = 0;
	SIZE s;
	char CH[2];
	CH[1] = '\0';
	r = { 0, 0, WALL, GROUND };
	HDC hdc = GetDC(hWnd);
	HideCaret(hWnd);
	bool filling = false;
	int c=0;

	if (Select)		///if text selected
	{
		SetBkColor(hdc, SelectColor);
		SetTextColor(hdc,SelectTextColor);
		swap(&MStart,&MEnd);		//if necessary
	}

	for (int i = 0; i < CountElements; i++)
	{
		if (i == 15)
		{
			c++;
		}
		DestroyCaret();
		CreateCar(hWnd);
		if (text[i] != '\r' && text[i] != '@')		//+ font, +image
		{
			if (text[i] == '_')
			{
				CH[0] = ' ';
			}
			else
				CH[0] = text[i];
			GetTextExtentPoint(hdc, (LPCWSTR)CH, 1, &s);		//get i-char size
			if (text[i] == '_')
				s.cx *= 8;
			if (MaxHght < s.cy)
				MaxHght = s.cy;

			if ((Curr.x + s.cx) < WALL)		//if within the window, then print, else -> \r
			{
				//if current point is start of selecting part, then filling->true
				if (Select && (Curr.x <= MStart.x && MStart.x < (Curr.x + s.cx)) && (Curr.y <= MStart.y && MStart.y < (Curr.y + MaxHght)))
				{
					if (Select)
					{
						SelectStart = i;
						filling = true;						
					}
				}
				//if current point is end of selecting part, then filling->false
				if ((Curr.x <= MEnd.x && MEnd.x < (Curr.x + s.cx)) && (Curr.y <= MEnd.y && MEnd.y < (Curr.y + MaxHght)))
				{
					SetCaretPos(Curr.x,Curr.y);
					CaretPos = i;
					SelectEnd = i;
					filling = false;
					break;
				}
				if (filling)
					TextOutA(hdc, Curr.x, Curr.y, (LPCSTR)CH, 1);
				Curr.x += s.cx;
			}
			else
			{
				Curr.y += MaxHght;
				MaxHght = 0;		//new line -> counter=0
				Curr.x = 0;

				if (filling)
					TextOutA(hdc, Curr.x, Curr.y, (LPCSTR)CH, 1);
				Curr.x += s.cx;
			}
		}
		else		//if '\r'
		{
			if (MaxHght == 0)
			{
				GetTextExtentPoint(hdc, (LPCWSTR)"a", 1, &s);
				MaxHght = s.cy;
			}
			Curr.y += MaxHght;
			MaxHght = 0;		//new line -> counter=0
			Curr.x = 0;
		}
	}

	if (SelectStart != 0 && SelectEnd == 0)
		SelectEnd = CountElements;

	if (!Select)
		ReDrawBox(hWnd);
	SetBkColor(hdc, BackColor);
	SetTextColor(hdc,TextColor);
	Click = false;
	ShowCaret(hWnd);
	ReleaseDC(hWnd, hdc);
}

void TextBox::swap(Point *a, Point *b)
{
	Point p;
	if ((*a).y > (*b).y || (((*a).y == (*b).y) && ((*a).x > (*b).x)))
	{
		p.x = (*a).x;
		p.y = (*a).y;

		(*a).x = (*b).x;
		(*a).y = (*b).y;

		(*b).x = p.x;
		(*b).y = p.y;
	}	
}