#pragma once

#include "ofConstants.h"
#include "ofEvents.h"

class ofGui {
public:
    ofGui(ofBaseRenderer* renderer);
    virtual ~ofGui();

    virtual void update();
    virtual void render();
    virtual void setup();
    virtual void reset();

    virtual void onMouseMoved(ofMouseEventArgs& event);
    virtual void onMouseDragged(ofMouseEventArgs& event);
    virtual void onMousePressed(ofMouseEventArgs& event);
    virtual void onMouseReleased(ofMouseEventArgs& event);
    virtual void onMouseScrolled(ofMouseEventArgs& event);
    virtual void mouseMoved(ofMouseEventArgs& event);
    virtual void touchDown(ofTouchEventArgs& touch);
    virtual void touchUp(ofTouchEventArgs& touch);
    virtual void touchMoved(ofTouchEventArgs& touch);
    
    virtual void onKeyPressed(ofKeyEventArgs& event);
    virtual void onKeyReleased(ofKeyEventArgs& event);
    virtual void onKeyChar(uint32_t& keyCode);

    virtual void onWindowResized(ofResizeEventArgs& window);

    virtual ImVec2 getMousePos();
    virtual void setMousePos(float x, float y);

    static const char* getClipboardString(void* user_data);
    static void setClipboardString(void* user_data, const char* text);

protected:
    void setStyle();

    bool mousePressed[5]  = { false, false, false, false, false };
    bool mouseReleased[5] = { true, true, true, true, true };
    ImVec2 mouseCursorPos = ImVec2(0, 0);

    ImFontAtlas* imAtlas;
    ofBaseRenderer* glRenderer;
};