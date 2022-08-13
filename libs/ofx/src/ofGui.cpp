#include "ofEventUtils.h"
#include "ofAppRunner.h"
#include "ofAppBaseWindow.h"
#include "ofBaseRenderer.h"
#include "ofGui.h"

using namespace std;

//--------------------------------------------------------------
ofGui::ofGui(ofBaseRenderer* renderer) : glRenderer(renderer) {

}
//--------------------------------------------------------------
ofGui::~ofGui() {

}

//--------------------------------------------------------------
void ofGui::update()
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = mouseCursorPos;

    for (int i = 0; i < 5; i++) {
        io.MouseDown[i] = mousePressed[i];
        mousePressed[i] = !mouseReleased[i];
    }

    ImGui::NewFrame();
}
//--------------------------------------------------------------
void ofGui::render()
{
    ImGui::Render();
}

//--------------------------------------------------------------
void ofGui::setup()
{
    imAtlas = new ImFontAtlas;

    const unsigned char ttf_font[] =
#                                   include "ttf_font.inl"

    ImFontConfig fontConfig;

    fontConfig.FontDataOwnedByAtlas = false;
    fontConfig.SizePixels = 13;
    fontConfig.FontDataSize = sizeof(ttf_font);

    imAtlas->AddFontFromMemoryTTF((void*)ttf_font, fontConfig.FontDataSize, fontConfig.SizePixels, &fontConfig);
    ImGui::CreateContext(imAtlas);
    setStyle();

    int w = 0, h = 0;
    unsigned char* pixels = nullptr;

    imAtlas->GetTexDataAsRGBA32(&pixels, &w, &h);
    imAtlas->TexID = (void*)glRenderer->createTexture(w, h, pixels);

    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab]         = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]   = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow]  = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]     = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]   = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]      = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown]    = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home]        = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End]         = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert]      = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]      = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]   = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]       = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]       = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]      = GLFW_KEY_ESCAPE;

    io.SetClipboardTextFn = &ofGui::setClipboardString;
    io.GetClipboardTextFn = &ofGui::getClipboardString;

    ofAddListener(ofEvents().keyReleased, this, &ofGui::onKeyReleased);

    ofAddListener(ofEvents().keyPressed,       this, &ofGui::onKeyPressed);
    ofAddListener(ofEvents().mouseMoved,       this, &ofGui::onMouseMoved);
    ofAddListener(ofEvents().mouseDragged,     this, &ofGui::onMouseDragged);
    ofAddListener(ofEvents().mousePressed,     this, &ofGui::onMousePressed);
    ofAddListener(ofEvents().mouseReleased,    this, &ofGui::onMouseReleased);
    ofAddListener(ofEvents().mouseScrolled,    this, &ofGui::onMouseScrolled);
    ofAddListener(ofEvents().windowResized,    this, &ofGui::onWindowResized);
}
//--------------------------------------------------------------
void ofGui::reset()
{
    ofRemoveListener(ofEvents().keyReleased, this, &ofGui::onKeyReleased);

    ofRemoveListener(ofEvents().keyPressed,    this, &ofGui::onKeyPressed);
    ofRemoveListener(ofEvents().mouseDragged,  this, &ofGui::onMouseDragged);
    ofRemoveListener(ofEvents().mousePressed,  this, &ofGui::onMousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofGui::onMouseReleased);
    ofRemoveListener(ofEvents().mouseScrolled, this, &ofGui::onMouseScrolled);
    ofRemoveListener(ofEvents().windowResized, this, &ofGui::onWindowResized);

    glRenderer->deleteTexture((unsigned)imAtlas->TexID);

    ImGui::DestroyContext();
    delete imAtlas;
}

//--------------------------------------------------------------
ImVec2 ofGui::getMousePos() {
    return mouseCursorPos;
}

//--------------------------------------------------------------
void ofGui::setMousePos(float x, float y) {
    mouseCursorPos.x = x;
    mouseCursorPos.y = y;
}

//--------------------------------------------------------------
void ofGui::onKeyPressed(ofKeyEventArgs& event) {
    int key = event.keycode;
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = true;
}

//--------------------------------------------------------------
void ofGui::onMousePressed(ofMouseEventArgs& event) {
    setMousePos(event.x, event.y);
    if (event.button >= 0 && event.button < 5) {
        mousePressed[event.button] = true;
        mouseReleased[event.button] = false;
    }
}

//--------------------------------------------------------------
void ofGui::onMouseReleased(ofMouseEventArgs& event) {
    setMousePos(event.x, event.y);
    mouseReleased[event.button] = true;
}

//--------------------------------------------------------------
void ofGui::mouseMoved(ofMouseEventArgs& event) {
    setMousePos((float)ofGetMouseX(), (float)ofGetMouseY());
}

//--------------------------------------------------------------
void ofGui::onMouseMoved(ofMouseEventArgs& event) {
    setMousePos(event.x, event.y);
}

//--------------------------------------------------------------
void ofGui::onMouseDragged(ofMouseEventArgs& event) {
    setMousePos(event.x, event.y);
    mouseReleased[event.button] = false;
}

//--------------------------------------------------------------
void ofGui::onMouseScrolled(ofMouseEventArgs& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = event.scrollY;
}

//--------------------------------------------------------------
void ofGui::touchDown(ofTouchEventArgs& touch) {
    setMousePos(touch.x, touch.y);
    mousePressed[0] = true;
    mouseReleased[0] = false;
}

//--------------------------------------------------------------
void ofGui::touchUp(ofTouchEventArgs& touch) {
    mouseReleased[0] = true;
}

//--------------------------------------------------------------
void ofGui::touchMoved(ofTouchEventArgs& touch) {
    setMousePos(touch.x, touch.y);
    mouseReleased[0] = false;
}
//--------------------------------------------------------------
void ofGui::onKeyReleased(ofKeyEventArgs& event)
{
    int key = event.keycode;
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = false;
    io.AddInputCharacter((unsigned short)event.codepoint);
}
//--------------------------------------------------------------
void ofGui::onWindowResized(ofResizeEventArgs& window) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)window.width, (float)window.height);
}

//--------------------------------------------------------------
const char* ofGui::getClipboardString(void* user_data) {
    return &ofGetWindowPtr()->getClipboardString()[0];
}

//--------------------------------------------------------------
void ofGui::setClipboardString(void* user_data, const char* text) {
    ofGetWindowPtr()->setClipboardString(text);
}
//--------------------------------------------------------------
void ofGui::setStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = ImVec4(0.72f, 0.84f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
}
//--------------------------------------------------------------