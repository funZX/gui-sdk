#include <imgui.h>
#include <imgui_internal.h>
#include "virtual_keyboard.h"

// ----------------------------------------------------------------------//
Gui::Keyboard::Keyboard(OnKeyFn fn)
    : Rect()
,     funcKey(fn)
{ 
    InputClear();
    keyShift = false;
}
// ----------------------------------------------------------------------//
void Gui::Keyboard::InputPush(const char c)
{
    inputBuf[inputBufSize++] = c;
}
// ----------------------------------------------------------------------//
void Gui::Keyboard::InputPop()
{
    if (inputBufSize > 2) // # SPACE
        inputBuf[--inputBufSize] = 0;
}
// ----------------------------------------------------------------------//
void Gui::Keyboard::Draw(Window& window, bool* p_open)
{
    ImGui::PushItemWidth(Width() - 2 * window.hSpace);
    if (ImGui::InputText("##CmdLine",
        inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
    {
    }
    ImGui::PopItemWidth();
    ImGui::SetItemDefaultFocus();

    ImGui::Separator();
    const std::string keyLine0 = "`1234567890-=";
    const std::string keyLine0Shift = "~!@#$%^&*()_+";
    const std::string keyLine1 = "qwertyuiop[]\\";
    const std::string keyLine1Shift = "QWERTYUIOP{}|";
    const std::string keyLine2 = "asdfghjkl;'";
    const std::string keyLine2Shift = "ASDFGHJKL:\"";
    const std::string keyLine3 = "zxcvbnm,./";
    const std::string keyLine3Shift = "ZXCVBNM<>?";

    const char* ctrlKeys[][2] = {
        {0          , "BACK" },
        {"TAB"      , "" },
        {"SHIFT"    , "ENTER" },
        {"CLEAR"    , "SPACE" },
    };

    const std::string keyBoard[][2] = {
        {keyLine0, keyLine0Shift},
        {keyLine1, keyLine1Shift},
        {keyLine2, keyLine2Shift},
        {keyLine3, keyLine3Shift},
    };

    const float btnSize = 35.0;
    EvType result = EvType::None;

    for (int k = 0; k < 4; k++)
    {
        const std::string& line = keyBoard[k][keyShift];

        const char* leftKey = ctrlKeys[k][0];
        const char* rightKey = ctrlKeys[k][1];

        if (leftKey)
        {
            if (ImGui::Button(leftKey, { 1.8f * btnSize, btnSize })) {
                
                EvType ev[] = {
                    EvType::None,
                    EvType::KeyTab,
                    EvType::KeyShift,
                    EvType::KeyClear
                };

                result = ev[k];

                if (result == EvType::KeyShift)
                    keyShift = !keyShift;
            }
            ImGui::SameLine();
        }

        for (int i = 0; i < line.size(); i++)
        {
            char btnName[2];
            btnName[0] = line[i];
            btnName[1] = 0;

            if (ImGui::Button(btnName, { btnSize, btnSize }))
                result = static_cast<EvType>(btnName[0]);

            ImGui::SameLine();
        }

        if (rightKey)
        {
            float btnWidth = (k < 2 ? 1.8 : (k < 3 ? 2.2 : 3.5)) * btnSize;
            if (ImGui::Button(rightKey, ImVec2({ btnWidth, btnSize }))) 
            {
                EvType ev[] = {
                    EvType::KeyBackspace,
                    EvType::None,
                    EvType::KeyEnter,
                    EvType::KeySpace
                };

                result = ev[k];
            }
        }
    }
    
    InputKey(result);
}
// ----------------------------------------------------------------------//
void Gui::Keyboard::InputClear()
{
    memset(inputBuf, 0, sizeof(inputBuf));
    inputBuf[0] = '#';
    inputBuf[1] = ' ';
    inputBufSize = 2;
}
// ----------------------------------------------------------------------//
bool Gui::Keyboard::InputKey(EvType ev)
{
    switch (ev)
    {
    case EvType::KeyTab:
        InputPush('\t');
        break;
    case EvType::KeyBackspace:
        InputPop();
        break;
    case EvType::KeyClear:
        funcKey(inputBuf, ev);
        InputClear();
        break;
    case EvType::KeyEnter:
        funcKey(inputBuf, ev);
        InputClear();
        break;
    case EvType::KeySpace:
        InputPush(' ');
        break;
    case EvType::None:
    case EvType::KeyShift:
        break;
    default:
        InputPush(static_cast<const char>(ev));
    }

    return ev == EvType::KeyEnter;
}
// ----------------------------------------------------------------------//