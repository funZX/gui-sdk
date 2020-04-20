#ifndef VIRTUAL_CONSOLE_H
#define VIRTUAL_CONSOLE_H

#include <iostream>
#include <vector>

#include "gui_wrap.h"
#include "gui_rect.h"
#include "virtual_keyboard.h"

namespace Gui
{
struct Console : public Rect
{
    Console();
    ~Console();

    void                Draw(Window& window, bool* p_open = 0);

protected:
    void                AddLine(const char* line);
    void                Clear();

protected:
    void Exec(const char* cmd);

    ImVector<char*>     buffer;
    bool                shouldScroll;

    Keyboard*           keyboard;
};
}
#endif