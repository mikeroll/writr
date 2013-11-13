#pragma once
class ImageList
{
private:
    SIZE imageSize;
    TCHAR imgName[MAX_PATH];    //file name
    int imgCount;

public:
     ImageList();
    ~ImageList();

    VOID LoadImageFromFile(HWND hWnd);
    
};

