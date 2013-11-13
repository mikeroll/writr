/* .wdoc format:


*/

#pragma once

#include "TextBox.h"

class WritrDocument
{
    LPCTSTR filename;
    HANDLE hFile;

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

    void CreateManifest();

public:

    WritrDocument(LPCTSTR filename, TextBox *editor);
    ~WritrDocument();

    HANDLE Open();
    void Close();
    void Flush();
    void Load();
};

