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
    mf.char_size = sizeof(TCHAR);
    mf.font_size = sizeof(BYTE);
    mf.doc_len = data->GetLength();
    mf.text_size = mf.doc_len * sizeof(TCHAR);
    mf.font_size = mf.doc_len * sizeof(BYTE);
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
    //SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    CreateManifest();
    DWORD nBytesWritten;
    WriteFile(hFile, &mf, mf_size, &nBytesWritten, NULL);                         //Write manifest
    WriteFile(hFile, data->GetTextPtr(), mf.text_size, &nBytesWritten, NULL);     //Write text
    WriteFile(hFile, data->GetFontPtr(), mf.font_size, &nBytesWritten, NULL);     //Write font data
    SetEndOfFile(hFile);
}

void WritrDocument::Load()
{
    DWORD nBytesRead;
    ReadFile(hFile, &mf, mf_size, &nBytesRead, NULL);                          //Read manifest
    ReadFile(hFile, data->GetTextPtr(), mf.text_size, &nBytesRead, NULL);      //Read text
    ReadFile(hFile, data->GetFontPtr(), mf.font_size, &nBytesRead, NULL);      //Read font data
	data->SetLength(mf.doc_len);
}