#ifndef GUI_WRAP_H
#define GUI_WRAP_H

#include <iostream>
#include <vector>
#include <imgui.h>

#include "gui_rect.h"

namespace Gui
{
    struct Window : public Rect
    {
        Window();

        bool  windowBlock;
        int   treeLevel;
        float hSpace;
        float vSpace;
        float hPadding;
        float vPadding;
    };

	bool BeginWindow(Window& window, const char* name, ImGuiWindowFlags flags, bool* open = nullptr);
	void EndWindow(Window& window);

    bool BeginTree(const char* name, Window& window);
    void EndTree(Window& window);
}
#endif 