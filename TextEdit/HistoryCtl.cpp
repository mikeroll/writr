#include "stdafx.h"
#include "HistoryCtl.h"


HistoryCtl::HistoryCtl(INT maxDepth)
{
    this->maxDepth = maxDepth;
    Clear();
}


HistoryCtl::~HistoryCtl()
{
}

VOID HistoryCtl::Clear()
{
    history.clear();
}

INT HistoryCtl::Memorize(EditorState state)
{
    history.push_back(state);
    if ((int)history.size() > maxDepth)
    {
        history.pop_front();
    }
    return history.size();
}

INT HistoryCtl::Trim(INT index)
{
    if (index <= (int)history.size())
    {
        history.resize(index + 1);
    }
    return history.size();
}
