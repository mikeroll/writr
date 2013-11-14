#include "stdafx.h"

#include "Dialogs.h"

std::wstring ChooseFile(FileAction action)
{
    OPENFILENAME ofn = { 0 };
    const DWORD maxFilename = 512;
    TCHAR filename[maxFilename];
    filename[0] = (TCHAR)0;
    TCHAR *filter;

    if (action == FA_ADDIMAGE)
        filter = L"Bitmap image\0*.bmp\0\0";
    else
        filter = L"Writr document\0*.wdoc\0\0";

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = maxFilename;
    ofn.lpstrFilter = filter;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (action == FA_SAVE)
    {
        if (!(GetSaveFileName(&ofn)))
            MessageBox(NULL, L"Error", L"Something went wrong. I'm sorry :(", MB_OK);
    }
    else
    {
        if (!(GetOpenFileName(&ofn)))
            MessageBox(NULL, L"Error", L"Something went wrong. I'm sorry :(", MB_OK);
    }
    std::wstring wFilename = filename;

    return wFilename;
}
