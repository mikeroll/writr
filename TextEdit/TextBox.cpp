#include "stdafx.h"
#include "TextBox.h" 
#include <Commdlg.h>

TextBox::TextBox(HWND hWnd)
{
    this->hWnd = hWnd;
    
    wall = 0;
    ground = 0;
    maxLineHeight = 0;

    Font[0] = { -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 33, _T("@Arial Unicode MS") };
    Font[1] = { -16, 0, 0, 0, 400, 0, 0, 0, 204, 3, 2, 1, 18, _T("Times New Roman") };
    Font[2] = { -16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 66, _T("Kristen ITC") };

    imgCount = 0;
    images = new ImageList();

    ResetState();
}

TextBox::~TextBox()
{
    delete images;
}

VOID TextBox::InsertChar(TCHAR ch)
{
    if (length < (MAX_LENGTH - 1))
    {
        for (int i = length; i >= caretPos; i--)
        {
            text[i + 1] = text[i];
            font[i + 1] = font[i];
        }
        text[caretPos] = (TCHAR)ch;
        font[caretPos] = CurrentFont;
        length++;
        if (caretPos<length)
            caretPos++;
    }
}

VOID TextBox::RemoveChar()      // Delete (not Backspace) one symbol
{
    for (int i = caretPos; i < length; i++)    //и чем тебе не понравился int =(
    {
        text[i] = text[i + 1];
        font[i] = font[i + 1];
    }
    if (caretPos != length)
        length--;
}

VOID TextBox::ResizeBox(LPARAM lParam)
{
    wall = LOWORD(lParam);
    ground = HIWORD(lParam);
}

VOID TextBox::ReDrawBox()
{
    BOOL isImage = false;
    int imgIndex = 0;
    Point Curr = { 0, 0 };
    maxLineHeight = 0;
    HFONT hFont;
    SIZE s;
    TCHAR CH[2];
    CH[1] = (TCHAR)0;
    r = { 0, 0, wall, ground };
    HDC hdc = GetDC(hWnd);
    HideCaret(hWnd);					
    SetBkMode(hdc, TRANSPARENT);
    FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW));		
    SetTextColor(hdc,TextColor);
    
    for (int i = 0; i < length; i++)
    {
        isImage = false;
        imgIndex = 0;
        DestroyCaret();
        Font[font[i]].lfHeight = zoom;
        hFont = CreateFontIndirectW(&Font[font[i]]);
        SelectObject(hdc,hFont);
        GetTextExtentPoint(hdc, (LPCTSTR)"A", 1, &s);
        CreateCar(s.cy);
        if (text[i] != '\r')		//+ font, +image
        {
            if ((text[i]>>8)==IMAGE)          //if image
            {
                isImage = true;
                imgIndex = text[i] & IMAGE;
            }

            if (!isImage)
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
            if (maxLineHeight < s.cy)
                maxLineHeight = s.cy;
            }
            else
            {
                images->GetImageSize(&s, imgIndex);
            }


            //TODO: do something with this mess
            //---becouse next letter eat some part of previous...
            s.cx += 2;  //Warning: sync. with SelectOrSetCaret()
            //---

            if ((Curr.x + s.cx) < wall)       //if within the window, then print, else -> \r
            {
                if (caretPos == i)           //Set Caret
                {	SetCaretPos(Curr.x, Curr.y);  }

                if (!isImage)
                {
                TextOut(hdc, Curr.x, Curr.y, CH, 1);
                }                    
                else
                {
                    images->DrawImage(hWnd, imgIndex, Curr.x, Curr.y);
                }
                Curr.x += s.cx;
            }
            else
            {
                Curr.y += maxLineHeight;
                maxLineHeight = 0;      //new line -> counter=0
                Curr.x = 0;

                if (caretPos == i)      //Set Caret
                {	SetCaretPos(Curr.x, Curr.y);	}

                if (!isImage)
                {
                TextOut(hdc, Curr.x, Curr.y, CH, 1);
                }
                else
                {
                    images->DrawImage(hWnd, imgIndex, Curr.x, Curr.y);
                }
                Curr.x += s.cx;
            }
        }
        else    //wrap text
        {
            if (caretPos == i)      //Set Caret
            {
                SetCaretPos(Curr.x, Curr.y);
            }

            if (maxLineHeight == 0)
            {
                GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
                maxLineHeight = s.cy;
            }
            Curr.y += maxLineHeight;
            maxLineHeight = 0;      //new line -> counter=0
            Curr.x = 0;
            Curr.y += maxLineHeight;
            maxLineHeight = 0;      //new line -> counter=0
        }
        DeleteObject(hFont);
    }

    if (caretPos == length)
    {
        if (Curr.x + 2 < wall)      //2 - caret's width
            SetCaretPos(Curr.x, Curr.y);
        else
        {
            if (maxLineHeight != 0)
                SetCaretPos(0, Curr.y + maxLineHeight);
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

VOID TextBox::KeyPress(WPARAM wParam)
{
    unsigned int key = wParam;
    switch (key)
    {
    case VK_TAB:
        InsertChar('_');
        break;
    default:
        InsertChar(key);
        break;
    }
    ReDrawBox();
    //TODO: add history
}

BOOL TextBox::SystemKey(WPARAM wParam)
{
    BOOL redraw = true;        //for ReDraw'ing window
    if ((GetKeyState(VK_CONTROL) < 0) && (LOWORD(wParam) == 0x41))		//Ctrl+A
    {
        selectStart = 0;
        selectEnd = length;
        isDblClicked = true;		//just becouse work as DBLClicK
        SelectOrSetCaret();
        redraw=false;
    }

    switch (wParam)
    {
    case VK_LEFT:
        MoveCar(LEFT);
        redraw = false;
        break;
    case VK_RIGHT:
        MoveCar(RIGHT);
        redraw = false;
        break;
    default:
        redraw = false;
    }
    return redraw;
}

VOID TextBox::MoveCar(Direction dir)
{
    switch (dir)
    {
    case LEFT:
        if (caretPos != 0)
        {
            caretPos--;
        }
        break;
    case RIGHT:
        if (caretPos < length)
        {
            caretPos++;
        }			
        break;
    case UP:
        break;
    case DOWN:
        break;
    }
    if (isSelected)
    {
        isDblClicked = false;
        isClicked = false;
        isSelected = false;
        selectStart = 0;
        selectEnd = 0;
        ReDrawBox();
    }
    else
        SelectOrSetCaret();
}

VOID TextBox::CreateCar(int height)
{
    HDC hdc = GetDC(hWnd);
    ReleaseDC(hWnd, hdc);
    CreateCaret(hWnd, NULL, 2, height);	
}

BOOL TextBox::MouseDown(LPARAM lParam)
{
    BOOL redraw = false;
    if (isSelected)
        redraw = true;
    MStart.x = LOWORD(lParam);
    MStart.y = HIWORD(lParam);
    MEnd.x = MStart.x;
    MEnd.y = MStart.y;
    isClicked = true;
    isDblClicked = false;
    isSelected = false;
    selectStart = 0;
    selectEnd = 0;
    SelectOrSetCaret();
    return redraw;
}

VOID TextBox::MouseUp(LPARAM lParam)
{
    isClicked = false;
    MEnd.x = LOWORD(lParam);
    MEnd.y = HIWORD(lParam);
    if (MStart.x != MEnd.x || MStart.y != MEnd.y)
    {
        isSelected = true;
    }
    if (!isDblClicked && isSelected)
    {
        SelectOrSetCaret();
    }        
    else
        isDblClicked = false;    
}

VOID TextBox::MouseMove(LPARAM lParam)
{
    if (isClicked)
    {
        MEnd.x = LOWORD(lParam);
        MEnd.y = HIWORD(lParam);
        if (MStart.x != MEnd.x && MStart.y != MEnd.y)
        {
            isSelected = true;
        }
        SelectOrSetCaret();
    }
}

VOID TextBox::SelectOrSetCaret()       //Difference with ReDrawBox(): all text not redraw - only selected part
{
    BOOL isImage = false;
    int imgIndex = 0;
    Point Curr = { 0, 0 };
    maxLineHeight = 0;
    SIZE s;
    HFONT hFont;
    TCHAR CH[2];
    CH[1] = (TCHAR)0;
    r = { 0, 0, wall, ground };
    HDC hdc = GetDC(hWnd);
    HideCaret(hWnd);
    bool filling = false;

    if (isSelected || isDblClicked)		///if text selected
    {
        SetBkColor(hdc, SelectColor);
        SetTextColor(hdc,SelectTextColor);
        swap(&MStart,&MEnd);		//if necessary
    }

    for (int i = 0; i < length; i++)
    {
        isImage = false;
        imgIndex = 0;
        DestroyCaret();
        Font[font[i]].lfHeight = zoom;                  //don't touch!
        hFont = CreateFontIndirectW(&Font[font[i]]);    //need for
        SelectObject(hdc, hFont);                       //caret position
        GetTextExtentPoint(hdc, (LPCTSTR)"A", 1, &s);
        CreateCar(s.cy);
        
        //----------------DoubleClick-------------
        if (isDblClicked && i == selectStart)
            filling = true;
        if (isDblClicked && i == selectEnd)
            filling = false;
        //------------------------------------------

        if (text[i] != '\r')		//+ font, +image
        {
            if ((text[i] >> 8) == IMAGE)          //if image
            {
                isImage = true;
                imgIndex = text[i] & IMAGE;
            }

            if (!isImage)
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
                if (maxLineHeight < s.cy)
                    maxLineHeight = s.cy;
            }
            else
            {
                images->GetImageSize(&s, imgIndex);
            }

            //---
            s.cx += 2;  //Warning: sync. with ReDrawBox()
            //---

            if ((Curr.x + s.cx) < wall)     //if within the window, then print, else -> \r
            {
                //if current point is start of selecting part, then filling->true
                if (isSelected && (Curr.x <= MStart.x && MStart.x < (Curr.x + s.cx)) && (Curr.y <= MStart.y && MStart.y < (Curr.y + maxLineHeight)))
                {
                    if (isSelected)
                    {
                        selectStart = i;
                        filling = true;
                    }
                }
                
                //if current point is end of selecting part, then filling->false
                if ( (isSelected || isClicked) && (Curr.x <= MEnd.x && MEnd.x < (Curr.x + s.cx)) && (Curr.y <= MEnd.y && MEnd.y < (Curr.y + maxLineHeight)))
                {
                    SetCaretPos(Curr.x,Curr.y);
                    caretPos = i;
                    selectEnd = i;
                    filling = false;
                    break;
                }

                //just for find caretPos position(<-,->)
                if ((!isClicked || isSelected) && caretPos == i)
                    SetCaretPos(Curr.x, Curr.y);

                if (filling)
                {
                    if (!isImage)
                    {
                        TextOut(hdc, Curr.x, Curr.y, CH, 1);
                    }
                    else
                    {
                        images->DrawImage(hWnd, imgIndex, Curr.x, Curr.y);
                    }
                }
                Curr.x += s.cx;
            }
            else
            {
                Curr.y += maxLineHeight;
                maxLineHeight = 0;      //new line -> counter=0
                Curr.x = 0;

                if (filling)
                {					
                    if (!isImage)
                    {
                        TextOut(hdc, Curr.x, Curr.y, CH, 1);
                    }
                    else
                    {
                        images->DrawImage(hWnd, imgIndex, Curr.x, Curr.y);
                    }
                }	

                //just for find caretPos position(<-,->)
                if ((!isClicked || isSelected) && caretPos == i)
                    SetCaretPos(Curr.x, Curr.y);
                Curr.x += s.cx;
            }
        }
        else		//if '\r'
        {
            //just for find caretPos position(<-,->)
            if ((!isClicked || isSelected) && caretPos == i)
                SetCaretPos(Curr.x, Curr.y);
            

            if (maxLineHeight == 0)
            {
                GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
                maxLineHeight = s.cy;
            }
            Curr.y += maxLineHeight;
            maxLineHeight = 0;		//new line -> counter=0
            Curr.x = 0;
        }
        DeleteObject(hFont);
    }

    if ((!isClicked || isSelected) && caretPos == length)       //just for find caretPos position(<-,->)
    {
        if (Curr.x + 2 < wall)		//2 - caret's width
            SetCaretPos(Curr.x, Curr.y);
        else
        {
            if (maxLineHeight != 0)
                SetCaretPos(0, Curr.y + maxLineHeight);
            else
            {
                GetTextExtentPoint(hdc, (LPCTSTR)"a", 1, &s);
                SetCaretPos(0, Curr.y + s.cy);
            }
        }
    }
    
    if ((MEnd.y > (Curr.y+s.cy)) || (MEnd.y == Curr.y && MEnd.x > Curr.x))
    {
        if (isClicked || isSelected || isDblClicked)
        {
            selectEnd = length;
            caretPos = selectEnd;
            SetCaretPos(Curr.x,Curr.y);
        }
    }
    
    if (isDblClicked)
        isSelected = true;

    SetTextColor(hdc,TextColor);
    ShowCaret(hWnd);
    ReleaseDC(hWnd, hdc);
}

VOID TextBox::swap(Point *a, Point *b)
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

VOID TextBox::ZoomIn()
{
    if (zoom > -96)
    {
        zoom--;
    }
}

VOID TextBox::ZoomOut()
{
    if (zoom < -8)
    {
        zoom++;
    }
}

BOOL TextBox::SetCurrentFont(BYTE f)
{
    BOOL redraw = false;
    if (f >= 0 && f < 3)
    {
        if (!isSelected)
            CurrentFont = f;
        else
        {
            for (int i = selectStart; i < selectEnd; i++)
            {
                font[i] = f;
            }
            redraw = true;
        }
    }
    selectStart = 0;
    selectEnd = 0;
    return redraw;		
}

VOID TextBox::SelectWord()
{
    int pos=caretPos;
    
    if (IsNormalChar(text[pos]))       //if text[pos] is a letter
    {
        int j = pos;
        //find left bound
        while (j >= 0 && j < length && IsNormalChar(text[j]))
        {
            selectStart = j;
            j--;
        }
        //find right bound
        j = pos;
        while (j >= 0 && j < length && IsNormalChar(text[j]))
        {
            selectEnd = j;
            j++;
        }
        selectEnd++;
        isDblClicked = true;
        SelectOrSetCaret();
    }
}

BOOL TextBox::IsNormalChar(TCHAR ch)
{
    BOOL isNormal = true;
    TCHAR specials[7] = {' ', ',', '.', '\r', '@', '_', (TCHAR)0 };
    for (int i = 0; i < 7; i++)
    {
        if (ch == specials[i])
            isNormal = false;
    }
    return isNormal;
}

VOID TextBox::Removing(WPARAM wParam)
{
    if (isSelected)
    {
        caretPos = selectStart;
        for (int i = 0; i < selectEnd - selectStart; i++)
        {
            RemoveChar();
        }
        isSelected = false;
        isDblClicked = false;
        selectStart = 0;
        selectEnd = 0;
    }
    else
    {
        if (wParam == VK_BACK)
            if (caretPos > 0) caretPos--;
        if (wParam == VK_DELETE) {}
        RemoveChar();

    }
    ReDrawBox();
}

std::wstring TextBox::GetSelection()
{
    if (isSelected)
    {
        std::wstring chunk(text[selectStart], selectEnd - selectStart);
        return chunk;
    }
    else return NULL;
}

VOID TextBox::InsertString(std::wstring s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        InsertChar(s[i]);
    }
}


VOID TextBox::InsertImage()
{
    if (images->LoadImageFromFile())
    {
        InsertChar((TCHAR)(0xff00 + imgCount));
        imgCount++;
        ReDrawBox();
    }
}


EditorState TextBox::GetState()
{
    EditorState state;
    memcpy(state.text, text, sizeof(TCHAR) * length);
    memcpy(state.font, font, sizeof(BYTE) * length);
    state.length = length;
    state.caretPos = caretPos;
    return state;
}

VOID TextBox::LoadState(EditorState state)
{
    length = state.length;
    memcpy(text, state.text, sizeof(TCHAR) * length);
    memcpy(font, state.font, sizeof(BYTE) * length);
    this->caretPos = state.caretPos;
    ReDrawBox();
}

VOID TextBox::ResetState()
{
    length = 0;
    caretPos = 0;
    text[length] = (TCHAR)0;
    font[length] = 0;

    isDblClicked = false;
    isSelected = false;
    isClicked = false;

    selectStart = 0;
    selectEnd = 0;
    MStart = { 0, 0 };
    MEnd = { 0, 0 };
    CurrentFont = 0;

    imgCount = 0;
    images = new ImageList();

    zoom = Font[CurrentFont].lfHeight;//don't write under the Font[]!!!

    ReDrawBox();
}
