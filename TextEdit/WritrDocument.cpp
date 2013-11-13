#include "stdafx.h"

#include "WritrDocument.h"


WritrDocument::WritrDocument(LPCTSTR filename, TextBox *editor)
{
    this->filename = filename;
    this->editor = editor;
}


WritrDocument::~WritrDocument()
{
}

void WritrDocument::CreateManifest()
{
    mf.charSize = sizeof(TCHAR);
    mf.fontSize = sizeof(BYTE);
    mf.docLen = state.length;
    mf.textSize = mf.docLen * sizeof(TCHAR);
    mf.fontSize = mf.docLen * sizeof(BYTE);
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
    state = editor->GetState();
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    CreateManifest();
    DWORD nBytesWritten;
    WriteFile(hFile, &mf, mfSize, &nBytesWritten, NULL);                 //Write manifest
    WriteFile(hFile, state.text, mf.textSize, &nBytesWritten, NULL);     //Write text
    WriteFile(hFile, state.font, mf.fontSize, &nBytesWritten, NULL);     //Write font data
    SetEndOfFile(hFile);
}

void WritrDocument::Load()
{
    DWORD nBytesRead;
    ReadFile(hFile, &mf, mfSize, &nBytesRead, NULL);                     //Read manifest
    ReadFile(hFile, state.text, mf.textSize, &nBytesRead, NULL);         //Read text
    ReadFile(hFile, state.font, mf.fontSize, &nBytesRead, NULL);         //Read font data
	state.length = mf.docLen;
    editor->LoadState(state);
}