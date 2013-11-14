#pragma once

#include <vector>

class ImageList
{
private:
    std::vector<HBITMAP> imgList;

public:
     ImageList();
    ~ImageList();

    BOOL LoadImageFromFile(std::wstring file);
    HBITMAP GatImageFromList(int index);
    VOID GetImageSize(SIZE *s, int index);
    VOID DrawImage(HWND hWnd, int index, int x, int y);    
};

