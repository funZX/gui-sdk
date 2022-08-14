#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
        void    setup(ofEventArgs& args);
        void    update(ofEventArgs& args);
        void    draw(ofEventArgs& args);
        void    exit(ofEventArgs& args);

        void    windowResized(ofResizeEventArgs& resize);
        void    fileDragged(ofDragInfo& drag);

        void    keyPressed(ofKeyEventArgs& key);
        void    keyReleased(ofKeyEventArgs& key);

        void    mouseMoved(ofMouseEventArgs& mouse);
        void    mouseDragged(ofMouseEventArgs& mouse);
        void    mousePressed(ofMouseEventArgs& mouse);
        void    mouseReleased(ofMouseEventArgs& mouse);
        void    mouseScrolled(ofMouseEventArgs& mouse);
        void    mouseEntered(ofMouseEventArgs& mouse);
        void    mouseExited(ofMouseEventArgs& mouse);
        void    messageReceived(ofMessage& message);

        void    touchDown(ofTouchEventArgs& touch);
        void    touchMoved(ofTouchEventArgs& touch);
        void    touchUp(ofTouchEventArgs& touch);
        void    touchDoubleTap(ofTouchEventArgs& touch);
        void    touchCancelled(ofTouchEventArgs& touch);
};
