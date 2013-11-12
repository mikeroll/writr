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
        UINT char_size;
        UINT doc_len;
        UINT text_size;
        UINT font_size;
    };
    UINT mf_size = sizeof(Manifest);
    Manifest mf;

    //Data to store
    TextBox *data;
    
    void CreateManifest();

public:
    WritrDocument(LPCTSTR filename, TextBox *editor);
    ~WritrDocument();

    HANDLE Open();
    void Close();
    void Flush();
    void Load();
};

