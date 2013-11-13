#include "stdafx.h"
#include "ImageList.h"
#include <Commdlg.h>
#include <winbase.h>


ImageList::ImageList()
{
    imgCount = 0;
}


ImageList::~ImageList()
{
}

/*
BOOL ImageList::LoadImageFromFile(HWND hWnd)
{
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
        imgCount++;
        bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), imgStruct.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  //load from file
        MT.imgStruct[MT.CaretPos - 1].B = (HBITMAP)LoadImage(GetModuleHandle(NULL), img.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); // загрузка картинки
        GetObject(MT.IMG[MT.CaretPos - 1].B, sizeof(BITMAP), &Bitmap);//берем структуру что бы узнать размер
        MT.IMG[MT.CaretPos - 1].BH = Bitmap.bmHeight; MT.IMG[MT.CaretPos - 1].BW = Bitmap.bmWidth;//размер
        isLoad = true;
    }
    else
    {
        MessageBox(NULL, (LPCWSTR)imgStruct.lpstrFile, L"ERROR: Couldn't load image!", MB_OK);
        isLoad = false;
    }
        
    return isLoad;
}
*/
