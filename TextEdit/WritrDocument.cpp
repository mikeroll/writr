#include "stdafx.h"
#include <atlconv.h>

#include "WritrDocument.h"


WritrDocument::WritrDocument(std::wstring defname, TextBox *editor)
{
    defaultName = defname;
    this->editor = editor;
    New();
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

std::wstring WritrDocument::CreateDefaultName()
{
    std::wstring newname = defaultName;
    newname.append(std::to_wstring(defNameCounter));
    newname.append(L".wdoc");
    defNameCounter++;
    return newname;
}

void WritrDocument::SetName(std::wstring filename)
{
    wFilename = std::wstring(filename);
    this->filename = wFilename.c_str();
}

void WritrDocument::New()
{
    SetName(CreateDefaultName());
    editor->ResetState();
}

HANDLE WritrDocument::Grab()
{
    HANDLE hFile = CreateFile(
        this->filename,
        GENERIC_ALL,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );
    return hFile;
}

void WritrDocument::Close(HANDLE hFile)
{
    CloseHandle(hFile);
}

void WritrDocument::Save()
{
    HANDLE hFile = Grab();
    state = editor->GetState();
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    CreateManifest();
    DWORD nBytesWritten;
    WriteFile(hFile, &mf, mfSize, &nBytesWritten, NULL);                 //Write manifest
    WriteFile(hFile, state.text, mf.textSize, &nBytesWritten, NULL);     //Write text
    WriteFile(hFile, state.font, mf.fontSize, &nBytesWritten, NULL);     //Write font data
    SetEndOfFile(hFile);
    Close(hFile);
}

void WritrDocument::Load()
{
    HANDLE hFile = Grab();
    DWORD nBytesRead;
    ReadFile(hFile, &mf, mfSize, &nBytesRead, NULL);                     //Read manifest
    ReadFile(hFile, state.text, mf.textSize, &nBytesRead, NULL);         //Read text
    ReadFile(hFile, state.font, mf.fontSize, &nBytesRead, NULL);         //Read font data
	state.length = mf.docLen;
    editor->ResetState();
    editor->LoadState(state);
    Close(hFile);
}