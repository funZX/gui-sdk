#pragma once

#include "ofConstants.h"
#include "ofWindowSettings.h"

class ofBaseApp;
class ofBaseRenderer;
class ofCoreEvents;

class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setup(const ofWindowSettings & settings)=0;
	virtual void update()=0;
	virtual void draw()=0;
	virtual bool getWindowShouldClose(){
		return false;
	}
	virtual void setWindowShouldClose(){
	}
	virtual void close(){
	}
	virtual ofCoreEvents & events() = 0;
	virtual std::shared_ptr<ofBaseRenderer> & renderer() = 0;

	virtual void hideCursor() {}
	virtual void showCursor() {}

	virtual void	setWindowPosition(int x, int y) {}
	virtual void	setWindowShape(int w, int h) {}

	virtual ImVec2	getWindowPosition() const {return ImVec2(); }
	virtual ImVec2	getWindowSize() const {return ImVec2(); }
	virtual ImVec2	getScreenSize() const {return ImVec2(); }

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth()  const { return 0; }
	virtual int		getHeight()  const { return 0; }

	virtual void	setWindowTitle(std::string title){}

	virtual ofWindowMode	getWindowMode() {return OF_WINDOW ;}

	virtual void	setFullscreen(bool fullscreen){}
	virtual void	toggleFullscreen(){}

	virtual void	enableSetupScreen(){}
	virtual void	disableSetupScreen(){}
	
	virtual void	setVerticalSync(bool enabled){};
    virtual void    setClipboardString(const std::string& text) {}
    virtual std::string  getClipboardString() { return ""; }

    virtual void makeCurrent(){};
	virtual void swapBuffers() {}
	virtual void startRender() {}
	virtual void finishRender() {}

    virtual void * getWindowContext(){return nullptr;};
};

class ofAppBaseGLESWindow: public ofAppBaseWindow{
public:
	virtual ~ofAppBaseGLESWindow(){}
	virtual void setup(const ofGLESWindowSettings & settings)=0;
	void setup(const ofWindowSettings & settings){
		const ofGLESWindowSettings * glSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
		if(glSettings){
			setup(*glSettings);
		}else{
			setup(ofGLESWindowSettings(settings));
		}
	}
};
