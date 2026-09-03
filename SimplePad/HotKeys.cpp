#include "HotKeys.h"

HotKeys::HotKeys()
{
}

HotKeys::~HotKeys()
{
}

std::optional<QKeyEvent*> HotKeys::keysProcessing(SimplePad* sp, QKeyEvent* pe)
{
    switch (pe->key())
    {
    case Qt::Key_O:
        if (pe->modifiers() & Qt::ControlModifier)
            sp->openFile();
        break;
    case Qt::Key_S:
        if (pe->modifiers() & Qt::ControlModifier)
            sp->saveFile();
        break;
    case Qt::Key_N:
        if (pe->modifiers() & Qt::ControlModifier)
            sp->newFile();
        break;
    case Qt::Key_Q:
        if (pe->modifiers() & Qt::ControlModifier)
            sp->close();
        break;
    default:
        return pe;
    }
        return std::nullopt;
}
