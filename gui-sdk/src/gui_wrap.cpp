#include "gui_wrap.h"


//--------------------------------------------------------------
Gui::Window::Window()
    : windowBlock(false)
    , treeLevel(0)
{
    ImGuiStyle& imStyle = ImGui::GetStyle();
    hSpace = imStyle.ItemSpacing.x;
    vSpace = imStyle.ItemSpacing.y;
    hPadding = imStyle.FramePadding.x;
    vPadding = imStyle.FramePadding.y;
}

//--------------------------------------------------------------
bool Gui::BeginWindow(Window& window, const char* name, ImGuiWindowFlags flags, bool * open)
{
	if (window.windowBlock)
		return false;

	window.windowBlock = true;

	ImGui::SetNextWindowPos(window.Position());
	ImGui::SetNextWindowSize(window.Size());
	return ImGui::Begin(name, open, flags | ImGuiWindowFlags_NoSavedSettings);
}

//--------------------------------------------------------------
void Gui::EndWindow(Window& window)
{
	if (!window.windowBlock)
		return;

	window.windowBlock = false;

	window.MoveTo(ImGui::GetWindowPos());
	window.Resize(ImGui::GetWindowSize());
	ImGui::End();
}
//--------------------------------------------------------------
bool Gui::BeginTree(const char* name, Window& window)
{
	bool result;
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (window.treeLevel == 0)
	{
		result = ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_CollapsingHeader);
	}
	else
	{
		result = ImGui::TreeNode(name);
	}

	window.treeLevel += result;

	return result;
}

//--------------------------------------------------------------
void Gui::EndTree(Window& window)
{
	window.treeLevel = window.treeLevel > 0 ? window.treeLevel - 1 : 0;

	ImGui::TreePop();
}

//--------------------------------------------------------------
