/* .wdoc format:

    0x00000000  UINT    text_start
    0x00000004  UINT    text_end
    0x00000008  UINT    font_start
    0x0000000C  UINT    font_end
    0x00000010  UINT    img_start
    0x00000014  UINT    img_end
    0x00000018  UINT    img_count
    text_start  WCHAR   text[0]
    ...
    text_end    WCHAR   text[length-1]
    font_start  BYTE    font[0]
    ...
    font_end    BYTE    font[length-1]


    (to be continued)
*/

#pragma once

#include "TextBox.h"

class WritrDocument
{
    LPCTSTR filename;
    HANDLE hFile;

    // Manifest
    struct Manifest {
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

