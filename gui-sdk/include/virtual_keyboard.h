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
        None, KeyTab, KeyShift, KeyClear, KeyBackspace, KeyEnter, KeySpace
    };

    typedef std::function<void(char*, EvType)> OnKeyFn;

    Keyboard(OnKeyFn fn);
    ~Keyboard() {}

    void Draw(Window& window, bool* p_open = 0);

protected:


    void                InputPush(const char c);
    void                InputPop();
    void                InputClear();

    bool                InputKey(EvType ev);
protected:
    bool                keyShift;

    char                inputBuf[512];
    int                 inputBufSize;

    OnKeyFn             funcKey;
};
}
#endif 