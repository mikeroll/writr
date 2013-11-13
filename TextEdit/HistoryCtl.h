#pragma once

#include <deque>

#include "TextBox.h"

class HistoryCtl
{
    TextBox *editor;
    INT maxDepth;
    INT currentStep;
    std::deque<EditorState> history;
public:
    HistoryCtl(TextBox *editor, INT maxDepth);
    ~HistoryCtl();

    INT Memorize();
    VOID Clear();

    VOID Undo();
    VOID Redo();
};

