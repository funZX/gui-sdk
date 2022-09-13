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
    io.DeltaTime = ofGetWindowPtr()->getDeltaTime();

    for (int i = 0; i < 5; i++)
        io.MouseDown[i] = mousePressed[i];

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
    fontConfig.SizePixels = 15;
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
    io.KeyMap[ImGuiKey_A]           = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C]           = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V]           = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X]           = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y]           = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z]           = GLFW_KEY_Z;

    io.SetClipboardTextFn = &ofGui::setClipboardString;
    io.GetClipboardTextFn = &ofGui::getClipboardString;
    io.ClipboardUserData  = this;

    ofAddListener(ofEvents().keyPressed,       this, &ofGui::onKeyPressed);
    ofAddListener(ofEvents().keyReleased,      this, &ofGui::onKeyReleased);
    ofAddListener(ofEvents().charEvent,        this, &ofGui::onKeyChar);

    ofAddListener(ofEvents().mouseMoved,       this, &ofGui::onMouseMoved);
    ofAddListener(ofEvents().mouseDragged,     this, &ofGui::onMouseDragged);
    ofAddListener(ofEvents().mousePressed,     this, &ofGui::onMousePressed);
    ofAddListener(ofEvents().mouseReleased,    this, &ofGui::onMouseReleased);
    ofAddListener(ofEvents().mouseScrolled,    this, &ofGui::onMouseScrolled);

    ofAddListener(ofEvents().touchDown,        this, &ofGui::touchDown);
    ofAddListener(ofEvents().touchUp,          this, &ofGui::touchUp);
    ofAddListener(ofEvents().touchMoved,       this, &ofGui::touchMoved);

    ofAddListener(ofEvents().windowResized,    this, &ofGui::onWindowResized);
}
//--------------------------------------------------------------
void ofGui::reset()
{
    ofRemoveListener(ofEvents().keyPressed,    this, &ofGui::onKeyPressed);
    ofRemoveListener(ofEvents().keyReleased,   this, &ofGui::onKeyReleased);
    ofRemoveListener(ofEvents().charEvent,     this, &ofGui::onKeyChar);

    ofRemoveListener(ofEvents().mouseDragged,  this, &ofGui::onMouseDragged);
    ofRemoveListener(ofEvents().mousePressed,  this, &ofGui::onMousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofGui::onMouseReleased);
    ofRemoveListener(ofEvents().mouseScrolled, this, &ofGui::onMouseScrolled);

    ofRemoveListener(ofEvents().touchDown,     this, &ofGui::touchDown);
    ofRemoveListener(ofEvents().touchUp,       this, &ofGui::touchUp);
    ofRemoveListener(ofEvents().touchMoved,    this, &ofGui::touchMoved);

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
    
    ImGuiIO& io = ImGui::GetIO();

    io.KeyCtrl = event.modifiers & OF_KEY_CONTROL;
    io.KeyShift = event.modifiers & OF_KEY_SHIFT;
    io.KeyAlt = event.modifiers & OF_KEY_ALT;
    io.KeySuper = event.modifiers & OF_KEY_SUPER;

    io.KeysDown[event.keycode] = true;
}
//--------------------------------------------------------------
void ofGui::onKeyReleased(ofKeyEventArgs& event)
{
    ImGuiIO& io = ImGui::GetIO();

    io.KeyCtrl = event.modifiers & OF_KEY_CONTROL;
    io.KeyShift = event.modifiers & OF_KEY_SHIFT;
    io.KeyAlt = event.modifiers & OF_KEY_ALT;
    io.KeySuper = event.modifiers & OF_KEY_SUPER;

    io.KeysDown[event.keycode] = false;
}
//--------------------------------------------------------------
void ofGui::onKeyChar(uint32_t& keyCode)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(keyCode);
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
    if (event.button >= 0 && event.button < 5) {
        mousePressed[event.button] = false;
        mouseReleased[event.button] = true;
    }
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
}

//--------------------------------------------------------------
void ofGui::onMouseScrolled(ofMouseEventArgs& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += event.scrollX;
    io.MouseWheel += event.scrollY;
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
void ofGui::onWindowResized(ofResizeEventArgs& window) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)window.width, (float)window.height);
}

//--------------------------------------------------------------
const char* ofGui::getClipboardString(void* user_data) {
    auto gui = static_cast<ofGui*>(user_data);
    gui->clipboardString = ofGetWindowPtr()->getClipboardString().c_str();

    return gui->clipboardString.c_str();
}

//--------------------------------------------------------------
void ofGui::setClipboardString(void* user_data, const char* text) {
    auto gui = static_cast<ofGui*>(user_data);
    gui->clipboardString = text;

    ofGetWindowPtr()->setClipboardString(text);
}
//--------------------------------------------------------------
void ofGui::setStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4 color_text = ImVec4(236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 1.0f);
    ImVec4 color_head = ImVec4(41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.0f);
    ImVec4 color_area = ImVec4(57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 1.0f);
    ImVec4 color_body = ImVec4(44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 1.0f);
    ImVec4 color_pops = ImVec4(33.f / 255.f, 46.f / 255.f, 60.f / 255.f, 1.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(color_text.x, color_text.y, color_text.z, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_text.x, color_text.y, color_text.z, 0.58f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(color_body.x, color_body.y, color_body.z, 0.95f);
    style.Colors[ImGuiCol_Border] = ImVec4(color_body.x, color_body.y, color_body.z, 0.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_body.x, color_body.y, color_body.z, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(color_area.x, color_area.y, color_area.z, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 0.78f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(color_area.x, color_area.y, color_area.z, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_area.x, color_area.y, color_area.z, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_area.x, color_area.y, color_area.z, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_area.x, color_area.y, color_area.z, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_head.x, color_head.y, color_head.z, 0.21f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(color_head.x, color_head.y, color_head.z, 0.80f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_head.x, color_head.y, color_head.z, 0.50f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(color_head.x, color_head.y, color_head.z, 0.50f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 0.86f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(color_head.x, color_head.y, color_head.z, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 0.86f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_head.x, color_head.y, color_head.z, 0.15f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(color_text.x, color_text.y, color_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_text.x, color_text.y, color_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_head.x, color_head.y, color_head.z, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_head.x, color_head.y, color_head.z, 0.43f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(color_pops.x, color_pops.y, color_pops.z, 0.92f);

    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
}
//--------------------------------------------------------------