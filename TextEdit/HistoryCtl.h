#pragma once

#include <deque>

#include "TextBox.h"

class HistoryCtl
{
    INT maxDepth;
    std::deque<EditorState> history;
public:
    HistoryCtl(INT maxDepth);
    ~HistoryCtl();

    INT Memorize(EditorState state);
    INT Trim(INT index);
    VOID Clear();
};

