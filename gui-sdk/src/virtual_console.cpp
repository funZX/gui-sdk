#include <imgui.h>

#include "gui_wrap.h"
#include "virtual_keyboard.h"
#include "virtual_console.h"

// ----------------------------------------------------------------------//
Gui::Console::Console() 
    : Rect()
{
    auto fn = [&](const char* inputBuf, Keyboard::EvType ev) {
        
        switch (ev)
        {
        case Keyboard::KeyEnter:
            AddOutput(inputBuf);
            Exec(&inputBuf[2]);
            break;
        case Keyboard::KeyClear:
            OutputClear();
            break;
        }
        shouldScroll = true;
    };

    keyboard = new Keyboard(fn);
    shouldScroll = false;
}
// ----------------------------------------------------------------------//
Gui::Console::~Console()
{
    delete keyboard;
}
// ----------------------------------------------------------------------//
void Gui::Console::AddOutput(const char* line)
{
    if (line[0])
        outputLines.push_back(line);
}
// ----------------------------------------------------------------------//

void Gui::Console::OutputClear()
{
    outputLines.clear();
}

// ----------------------------------------------------------------------//
void Gui::Console::Draw(Window& window, bool* p_open)
{
    ImGui::BeginChild("Output", {0, -window.vSpace}, false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    for (int i = 0; i < outputLines.Size; i++)
    {
        const char* item = outputLines[i].c_str();
        ImGui::TextUnformatted(item);
    }
    ImGui::PopStyleVar();

    if (shouldScroll || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    shouldScroll = false;

    ImGui::EndChild();

    if (ImGui::IsItemClicked())
        ImGui::OpenPopup("##Keyboard");
    ImGui::SameLine();
    if (ImGui::BeginPopup("##Keyboard"))
    {
        keyboard->Draw(window);
        ImGui::EndPopup();
    }
}

// ----------------------------------------------------------------------//

void Gui::Console::Exec(const char* cmd)
{
    
}
// ----------------------------------------------------------------------//
