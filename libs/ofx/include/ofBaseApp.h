#pragma once


class ofResizeEventArgs;
class ofEventArgs;
class ofMouseEventArgs;
class ofKeyEventArgs;
class ofDragInfo;
class ofMessage;
class ofTouchEventArgs;

class ofBaseApp {

	public:
		ofBaseApp();

		virtual ~ofBaseApp();

		virtual void setup(ofEventArgs & args) = 0;
		virtual void update(ofEventArgs & args) = 0;
		virtual void draw(ofEventArgs & args) = 0;
		virtual void exit(ofEventArgs & args) = 0;

		virtual void windowResized(ofResizeEventArgs & resize){}
		virtual void fileDragged(ofDragInfo& drag) {}

		virtual void keyPressed( ofKeyEventArgs & key ) {}
		virtual void keyReleased( ofKeyEventArgs & key ) {}

		virtual void mouseMoved( ofMouseEventArgs & mouse ) {}
		virtual void mouseDragged( ofMouseEventArgs & mouse ) {}
		virtual void mousePressed( ofMouseEventArgs & mouse ) {}
		virtual void mouseReleased( ofMouseEventArgs & mouse ) {}
		virtual void mouseScrolled( ofMouseEventArgs & mouse ) {}
		virtual void mouseEntered( ofMouseEventArgs & mouse ) {}
		virtual void mouseExited( ofMouseEventArgs & mouse ) {}
		virtual void messageReceived( ofMessage & message ) {}

		virtual void touchDown(ofTouchEventArgs & touch) {}
		virtual void touchMoved(ofTouchEventArgs & touch) {}
		virtual void touchUp(ofTouchEventArgs & touch) {}
		virtual void touchDoubleTap(ofTouchEventArgs & touch) {}
		virtual void touchCancelled(ofTouchEventArgs & touch) {}
};


