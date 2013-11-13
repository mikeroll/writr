#include "stdafx.h"
#include "ImageList.h"
#include <Commdlg.h>
#include <winbase.h>


ImageList::ImageList()
{
}

ImageList::~ImageList()
{
}

BOOL ImageList::LoadImageFromFile()
{
    TCHAR imgName[MAX_PATH];    //file name(used for init.)
    HBITMAP bmp;
    BOOL isLoad=false;
    OPENFILENAME  imgStruct;
    SecureZeroMemory(&imgStruct, sizeof(imgStruct));
    imgStruct.lStructSize = sizeof (imgStruct);
    imgStruct.hwndOwner = NULL;
    imgStruct.lpstrFile = (LPWSTR)imgName;
    imgStruct.lpstrFile[0] = '\0';
    imgStruct.nMaxFile = sizeof(imgName);
    imgStruct.lpstrFilter = L"All\0*.*\0Image\0*.jpg\0";
    imgStruct.nFilterIndex = 1;
    imgStruct.lpstrFileTitle = NULL;
    imgStruct.nMaxFileTitle = 0;
    imgStruct.lpstrInitialDir = NULL;
    imgStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&imgStruct))
    {
        MessageBox(NULL, (LPCWSTR)imgStruct.lpstrFile, L"Image Successful Inserting", MB_OK);

        
        //insert img to list
        
        bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), imgStruct.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  //load from file
        if (bmp)
        {
            imgList.insert(imgList.end(), bmp);
            isLoad = true;
        }
        else
        {
            MessageBox(NULL, (LPCWSTR)imgStruct.lpstrFile, L"ERROR: Couldn't load image!", MB_OK);
            isLoad = false;
        }
    }
    else
    {
        MessageBox(NULL, (LPCWSTR)imgStruct.lpstrFile, L"ERROR: Couldn't open image!", MB_OK);
        isLoad = false;
    }
        
    return isLoad;
}

HBITMAP ImageList::GatImageFromList(int index)
{
    std::list<HBITMAP>::iterator iterator;
    HBITMAP bmp;
    iterator = imgList.begin();
    std::advance(iterator, index);
    bmp = *iterator;
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




