#include "stdafx.h"

#include "WritrDocument.h"


WritrDocument::WritrDocument(LPCTSTR filename, TextBox *editor)
{
    this->filename = filename;
    this->data = editor;
}


WritrDocument::~WritrDocument()
{
}

void WritrDocument::CreateManifest()
{
    mf.text_size = data->GetLength() * sizeof(TCHAR);
    mf.font_size = data->GetLength() * sizeof(BYTE);
}

HANDLE WritrDocument::Open()
{
    HANDLE hFile = CreateFile(
        filename,
        GENERIC_ALL,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );
    this->hFile = hFile;
    return hFile;
}

void WritrDocument::Close()
{
    CloseHandle(hFile);
}

void WritrDocument::Flush()
{
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    CreateManifest();
    WriteFile(hFile, &mf, mf_size, NULL, NULL);                         //Write manifest
    WriteFile(hFile, data->GetTextPtr(), mf.text_size, NULL, NULL);     //Write text
    WriteFile(hFile, data->GetFontPtr(), mf.font_size, NULL, NULL);     //Write font data
    SetEndOfFile(hFile);
}

void WritrDocument::Load()
{
    ReadFile(hFile, &mf, mf_size, NULL, NULL);                          //Read manifest
    ReadFile(hFile, data->GetTextPtr(), mf.text_size, NULL, NULL);      //Read text
    ReadFile(hFile, data->GetFontPtr(), mf.font_size, NULL, NULL);      //Read font data
}