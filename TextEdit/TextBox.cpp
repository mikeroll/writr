#include "stdafx.h"
#include "TextBox.h" 


TextBox::TextBox()
{
	WALL = 0;
	GROUND = 0;
	MaxHght = 0;
	CaretPos = 0;
	text[0] = '\0';
	font[0] = 0;
	CountElements = 0;
	SelectStart = 0;
	SelectEnd = 0;
	DBLClicK = false;
	Select = false;
	Click = false;
	MStart = {0,0};
	MEnd = {0,0};
	CurrentFont = 0;
	Font[0] = { -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 33, _T("@Arial Unicode MS") };
	Font[1] = { -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 18, _T("Times New Roman") };
	Font[2] = { -16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 66, _T("Kristen ITC") };
	zoom = Font[CurrentFont].lfHeight;//don't write under the Font[]!!!

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
			font[i + 1] = font[i];
		}
		text[CaretPos] = ch;
		font[CaretPos] = CurrentFont;
		CountElements++;
		if (CaretPos<CountElements)
			CaretPos++;
	}
}

void TextBox::RemoveText()		// Delete (not Backspace) one symbol
{
	for (int i = CaretPos; i<CountElements; i++)
	{
		text[i] = text[i + 1];
		font[i] = font[i + 1];
	}
	if (CaretPos!=CountElements)
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
	HFONT hFont;
	SIZE s;
	TCHAR CH[2];
	CH[1] = '\0';
	r = {0,0,WALL,GROUND};
	HDC hdc = GetDC(hWnd);
	HideCaret(hWnd);					
	//SetBkColor(hdc,BackColor);
	SetBkMode(hdc, TRANSPARENT);
	FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW));		
	SetTextColor(hdc,TextColor);
	
	for (int i = 0; i < CountElements; i++)
	{
		DestroyCaret();
		Font[font[i]].lfHeight = zoom;
		hFont = CreateFontIndirectW(&Font[font[i]]);
		SelectObject(hdc,hFont);
		GetTextExtentPoint(hdc, (LPCTSTR)"A", 1, &s);
		CreateCar(hWnd, s.cy);
		if (text[i] != '\r' && text[i] != '@')		//+ font, +image
		{
			if (text[i] == '_')
			{
				CH[0] = ' ';
			}
			else 
				CH[0] = text[i];
			GetTextExtentPoint(hdc, (LPCTSTR)CH, 1, &s);		//get i-char size
			if (text[i] == '_')
				s.cx *= 8;
			if (MaxHght < s.cy)
				MaxHght=s.cy;

			//---becouse next letter eat some part of previous...
			s.cx += 2;					//Warning: sync. with SelectOrSetCaret()
			//---

			if ((Curr.x+s.cx) < WALL)		//if within the window, then print, else -> \r
			{
				if (CaretPos==i)		//Set Caret
				{	SetCaretPos(Curr.x, Curr.y);  }

				TextOut(hdc, Curr.x,Curr.y, CH, 1);
				Curr.x += s.cx;
			}
			else
			{
				Curr.y += MaxHght;
				MaxHght = 0;		//new line -> counter=0
				Curr.x = 0;

				if (CaretPos == i)		//Set Caret
				{	SetCaretPos(Curr.x, Curr.y);	}

				TextOut(hdc, Curr.x, Curr.y, CH, 1);
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
				GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
				MaxHght = s.cy;
			}
			Curr.y += MaxHght;
			MaxHght = 0;		//new line -> counter=0
			Curr.x = 0;
		}
		DeleteObject(hFont);
	}

	if (CaretPos == CountElements)
	{
		if (Curr.x + 2 < WALL)		//2 - caret's width
			SetCaretPos(Curr.x,Curr.y);
		else
		{
			if (MaxHght!=0)
				SetCaretPos(0,Curr.y+MaxHght);
			else
			{
				GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
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

int TextBox::SystemKey(WPARAM wParam, HWND hWnd)
{
	int result = 1;		//for ReDraw'ing window
	if ((GetKeyState(VK_CONTROL) < 0) && (LOWORD(wParam) == 0x41))		//Ctrl+A
	{
		SelectStart = 0;
		SelectEnd = CountElements;
		DBLClicK = true;		//just becouse work as DBLClicK
		SelectOrSetCaret(hWnd);
		result=0;
	}

	switch (wParam)
	{
	case VK_LEFT:
		MoveCar('l', hWnd);
		result = 0;
		break;
	case VK_RIGHT:
		MoveCar('r',hWnd);
		result = 0;
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
			DBLClicK = false;
			SelectStart = 0;
			SelectEnd = 0;
		}
		break;
	default:
		result = 0;
	}
	return result;
}

void TextBox::MoveCar(char dir, HWND hWnd)
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
	if (Select)
	{
		Click = false;
		Select = false;
		SelectStart = 0;
		SelectEnd = 0;
		ReDrawBox(hWnd);
	}
	else
		SelectOrSetCaret(hWnd);
}

void TextBox::CreateCar(HWND hWnd,int height)
{
	HDC hdc = GetDC(hWnd);
	ReleaseDC(hWnd, hdc);
	CreateCaret(hWnd, NULL, 2, height);	
}

void TextBox::MouseDown(LPARAM lParam)
{
	MStart.x = LOWORD(lParam);
	MStart.y = HIWORD(lParam);
	Click = true;
	DBLClicK = false;
	Select = false;
	SelectStart = 0;
	SelectEnd = 0;
}

void TextBox::MouseUp(LPARAM lParam,HWND hWnd)
{
	//Click = false;
	MEnd.x = LOWORD(lParam);
	MEnd.y = HIWORD(lParam);
	if (MStart.x != MEnd.x || MStart.y != MEnd.y)
	{
		Select = true;		
	}
	if (!DBLClicK)
		SelectOrSetCaret(hWnd);
	else
		DBLClicK = false;
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
		//SelectOrSetCaret(hWnd);
	}
}

void TextBox::SelectOrSetCaret(HWND hWnd)		//Difference with ReDrawBox(): all text not redraw - only selected part
{
	Point Curr = { 0, 0 };
	MaxHght = 0;
	SIZE s;
	HFONT hFont;
	TCHAR CH[2];
	CH[1] = '\0';
	r = { 0, 0, WALL, GROUND };
	HDC hdc = GetDC(hWnd);
	HideCaret(hWnd);
	bool filling = false;

	if (Select || DBLClicK)		///if text selected
	{
		SetBkColor(hdc, SelectColor);
		SetTextColor(hdc, SelectTextColor);
		swap(&MStart, &MEnd);		//if necessary
	}

	for (int i = 0; i < CountElements; i++)
	{
		DestroyCaret();
		Font[font[i]].lfHeight = zoom;					//don't touch!  
		hFont = CreateFontIndirectW(&Font[font[i]]);	//need for
		SelectObject(hdc, hFont);						//caret position
		GetTextExtentPoint(hdc, (LPCTSTR)"A", 1, &s);
		CreateCar(hWnd, s.cy);

		//----------------DoubleClick-------------
		if (DBLClicK && i == SelectStart)
			filling = true;
		if (DBLClicK && i == SelectEnd)
			filling = false;
		//------------------------------------------

		if (text[i] != '\r' && text[i] != '@')		//+ font, +image
		{
			if (text[i] == '_')
			{
				CH[0] = ' ';
			}
			else
				CH[0] = text[i];
			GetTextExtentPoint(hdc, (LPCTSTR)CH, 1, &s);		//get i-char size
			if (text[i] == '_')
				s.cx *= 8;
			if (MaxHght < s.cy)
				MaxHght = s.cy;

			//---
			s.cx += 2;		//Warning: sync. with ReDrawBox()
			//---

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
				if ((Select || Click) && (Curr.x <= MEnd.x && MEnd.x < (Curr.x + s.cx)) && (Curr.y <= MEnd.y && MEnd.y < (Curr.y + MaxHght)))
				{
					SetCaretPos(Curr.x, Curr.y);
					CaretPos = i;
					SelectEnd = i;
					filling = false;
					break;
				}

				//just for find CaretPos position(<-,->)
				if ((!Click || Select) && CaretPos == i)
					SetCaretPos(Curr.x, Curr.y);

				if (filling)
				{
					TextOut(hdc, Curr.x, Curr.y, (LPCTSTR)CH, 1);
				}

				Curr.x += s.cx;
			}
			else
			{
				Curr.y += MaxHght;
				MaxHght = 0;		//new line -> counter=0
				Curr.x = 0;

				if (filling)
				{
					TextOut(hdc, Curr.x, Curr.y, (LPCTSTR)CH, 1);
				}

				//just for find CaretPos position(<-,->)
				if ((!Click || Select) && CaretPos == i)
					SetCaretPos(Curr.x, Curr.y);
				Curr.x += s.cx;
			}
		}
		else		//if '\r'
		{
			//just for find CaretPos position(<-,->)
			if ((!Click || Select) && CaretPos == i)
				SetCaretPos(Curr.x, Curr.y);

			if (MaxHght == 0)
			{
				GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
				MaxHght = s.cy;
			}
			Curr.y += MaxHght;
			MaxHght = 0;		//new line -> counter=0
			Curr.x = 0;
		}
		DeleteObject(hFont);
	}

	if ((!Click || Select) && CaretPos == CountElements)		//just for find CaretPos position(<-,->)
	{
		if (Curr.x + 2 < WALL)		//2 - caret's width
			SetCaretPos(Curr.x, Curr.y);
		else
		{
			if (MaxHght != 0)
				SetCaretPos(0, Curr.y + MaxHght);
			else
			{
				GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
				SetCaretPos(0, Curr.y + s.cy);
			}
		}
	}

	if ((Click || Select || DBLClicK) && (SelectEnd == 0))
	{
		SelectEnd = CountElements; //if selected part go trough the end of text
		CaretPos = SelectEnd;
		SetCaretPos(Curr.x,Curr.y);
	}
		

	if (DBLClicK)
		Select = true;
//	SetBkColor(hdc, BackColor);
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

void TextBox::WheelUP()
{
	if (zoom > -96)
	{
		zoom--;		//make it bigger
	}
}

void TextBox::WheelDN()
{
	if (zoom < -8)
	{
		zoom++;		//make it smaller
	}
}

int TextBox::SetCurrentFont(BYTE f)
{
	int result = 0;		//not reDraw
	if (f >= 0 && f < 3)
	{
		if (!Select)
			CurrentFont = f;
		else
		{
			for (int i = SelectStart; i < SelectEnd; i++)
			{
				font[i] = f;
			}
			result = 1;
		}
	}
	SelectStart = 0;
	SelectEnd = 0;
	return result;		
}

void TextBox::SelectWord(HWND hWnd)
{
	int pos=CaretPos;
	
	if (IsNormalChar(text[pos]))		//if text[pos] is a letter
	{
		int j = pos;
		//go to left
		while (j>=0 && j<CountElements && IsNormalChar(text[j]))
		{
			SelectStart = j;
			j--;
		}
		//go to right
		j = pos;
		while (j >= 0 && j<CountElements && IsNormalChar(text[j]))
		{
			SelectEnd = j;
			j++;
		}
		SelectEnd++;
		DBLClicK = true;
		SelectOrSetCaret(hWnd);
	}
}

int TextBox::IsNormalChar(TCHAR ch)
{
	int result=1;
	TCHAR C[7] = {' ', ',', '.', '\r', '@', '_', '\0'};
	for (int i = 0; i < 7; i++)
	{
		if (ch == C[i])
			result = 0;
	}
	return result;
}

UINT TextBox::GetLength()
{
    return (UINT)CountElements;
}