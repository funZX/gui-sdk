#ifndef VIRTUAL_KEYBOAD_H
#define VIRTUAL_KEYBOAD_H

#include <functional>

#include "gui_wrap.h"
#include "gui_rect.h"

namespace Gui
{
struct Keyboard : public Rect
{
    enum EvType {
        None, KeyTab, KeyChar, KeyShift, KeyClear, KeyBackspace, KeyEnter, KeySpace
    };

    typedef std::function<void(EvType, int key)> OnKeyFn;

    Keyboard();
    ~Keyboard() {}

    bool                Open(Window& window, OnKeyFn fn = OnKeyFn());

protected:
    bool                Draw(Window& window, OnKeyFn fn);

    void                InputPush(const char c);
    void                InputPop();
    void                InputClear();

    void                InputKey(EvType ev, int key);
protected:
    bool                keyShift;

    char                inputBuf[512];
    int                 inputBufSize;
};
}
#endif 