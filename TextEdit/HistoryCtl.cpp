#include "stdafx.h"
#include "HistoryCtl.h"


HistoryCtl::HistoryCtl(TextBox *editor, INT maxDepth)
{
    this->editor = editor;
    this->maxDepth = maxDepth;
    this->currentStep = -1;
    Clear();
}


HistoryCtl::~HistoryCtl()
{
}

VOID HistoryCtl::Clear()
{
    history.clear();
}

INT HistoryCtl::Memorize()
{
    history.resize(currentStep + 1);
    history.push_back(editor->GetState());
    currentStep++;
    if (currentStep+1 > maxDepth)
    {
        history.pop_front();
        currentStep--;
    }
    return history.size();
}

VOID HistoryCtl::Undo()
{
    if (currentStep > 0) {
        editor->LoadState(history[currentStep-1]);
        currentStep--;
    }
}

VOID HistoryCtl::Redo()
{
    if (currentStep < (int)history.size() - 1) {
        editor->LoadState(history[currentStep + 1]);
        currentStep++;
    }
}