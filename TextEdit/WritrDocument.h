/* .wdoc format:


*/

#pragma once

#include <string>

#include "TextBox.h"

class WritrDocument
{
    std::wstring defaultName;
    int defNameCounter = 0;
    std::wstring wFilename;
    LPCTSTR filename;

    // Manifest
    struct Manifest {
        UINT charSize;
        UINT docLen;
        UINT textSize;
        UINT fontSize;
    };
    UINT mfSize = sizeof(Manifest);
    Manifest mf;

    //Data to store
    EditorState state;

    TextBox *editor;

    std::wstring CreateDefaultName();
    void CreateManifest();

    HANDLE Grab();
    void Close(HANDLE hFile);

public:

    WritrDocument(std::wstring defname, TextBox *editor);
    ~WritrDocument();

    void SetName(std::wstring filename);
    void New();
    void Save();
    void Load();
};

