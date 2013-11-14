#include "stdafx.h"

#include "ImageList.h"

ImageList::ImageList()
{
}

ImageList::~ImageList()
{
}

BOOL ImageList::LoadImageFromFile(std::wstring filename)
{
    HBITMAP bmp;
    BOOL isLoaded=false;
    LPCTSTR file = filename.c_str();
        
    //insert img to list
        
    bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  //load from file
    if (bmp)
    {
        imgList.push_back(bmp);
        isLoaded = true;
    }
    else
    {
        MessageBox(NULL, file, L"ERROR: Couldn't load image!", MB_OK);
        isLoaded = false;
    }

    return isLoaded;
}

HBITMAP ImageList::GatImageFromList(int index)
{    
    HBITMAP bmp = imgList[index];
    return bmp;
}

VOID ImageList::GetImageSize(SIZE *s, int index)
{
    BITMAP bmpStruct;
    GetObject(GatImageFromList(index), sizeof(BITMAP), &bmpStruct);
    (*s).cx = bmpStruct.bmWidth;
    (*s).cy = bmpStruct.bmWidth;
}

VOID ImageList::DrawImage(HWND hWnd, int index,int x, int y)
{
    SIZE s;
    GetImageSize(&s,index);
    HDC hdc = GetDC(hWnd);
    HDC imageDc = CreateCompatibleDC(hdc);
    SelectObject(imageDc, GatImageFromList(index));
    BitBlt(hdc, x, y, s.cx, s.cy, imageDc, 0, 0, SRCCOPY);// рисуем картинку
    ReleaseDC(hWnd, hdc);
    DeleteDC(imageDc);
}

VOID ImageList::RemoveImage()
{
    imgList.erase(imgList.end());
}




