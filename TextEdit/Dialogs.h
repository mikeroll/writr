#pragma once

#include <string>
#include <Commdlg.h>

enum FileAction { FA_OPEN, FA_SAVE, FA_ADDIMAGE };
std::wstring ChooseFile(FileAction action);
