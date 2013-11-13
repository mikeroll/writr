#pragma once
#include <list>
//#include <iostream>
class ImageList
{
private:
    SIZE imageSize;
    TCHAR imgName[MAX_PATH];    //file name
    int imgCount;
    std::list<HBITMAP> imgList;
    std::list<HBITMAP>::iterator iterator;

public:
     ImageList();
    ~ImageList();

    BOOL LoadImageFromFile(HWND hWnd);
    
};

