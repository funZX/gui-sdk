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

		virtual void windowResized(ofResizeEventArgs & resize) = 0;
		virtual void fileDragged(ofDragInfo& drag) = 0;

		virtual void keyPressed( ofKeyEventArgs & key ) = 0;
		virtual void keyReleased( ofKeyEventArgs & key ) = 0;

		virtual void mouseMoved( ofMouseEventArgs & mouse ) = 0;
		virtual void mouseDragged( ofMouseEventArgs & mouse ) = 0;
		virtual void mousePressed( ofMouseEventArgs & mouse ) = 0;
		virtual void mouseReleased( ofMouseEventArgs & mouse ) = 0;
		virtual void mouseScrolled( ofMouseEventArgs & mouse ) = 0;
		virtual void mouseEntered( ofMouseEventArgs & mouse ) = 0;
		virtual void mouseExited( ofMouseEventArgs & mouse ) = 0;
		virtual void messageReceived( ofMessage & message ) = 0;

		virtual void touchDown(ofTouchEventArgs & touch) = 0;
		virtual void touchMoved(ofTouchEventArgs & touch) = 0;
		virtual void touchUp(ofTouchEventArgs & touch) = 0;
		virtual void touchDoubleTap(ofTouchEventArgs & touch) = 0;
		virtual void touchCancelled(ofTouchEventArgs & touch) = 0;
};


