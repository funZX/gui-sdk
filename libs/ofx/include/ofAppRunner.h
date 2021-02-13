#pragma once

#include "ofConstants.h"
#include "ofWindowSettings.h"
#include "ofMainLoop.h"
#include "ofRectangle.h"

class ofAppBaseWindow;
class ofAppBaseGLWindow;
class ofAppBaseGLESWindow;
class ofAppGLFWWindow;
class ofBaseApp;
class ofBaseRenderer;
class ofCoreEvents;


void ofInit();
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode);	// sets up the opengl context!
std::shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings);	// sets up the opengl context!
std::shared_ptr<ofMainLoop> ofGetMainLoop();
void ofSetMainLoop(std::shared_ptr<ofMainLoop> mainLoop);

template<typename Window>
void ofSetupOpenGL(std::shared_ptr<Window> windowPtr, int w, int h, ofWindowMode screenMode){
	ofInit();
	ofWindowSettings settings;
	settings.setSize(w, h);
	settings.windowMode = screenMode;
	ofGetMainLoop()->addWindow(windowPtr);
	windowPtr->setup(settings);
}

//special case so we preserve supplied settngs
//TODO: remove me when we remove the ofSetupOpenGL legacy approach.
void ofSetupOpenGL(std::shared_ptr<ofAppGLFWWindow> windowPtr, int w, int h, ofWindowMode screenMode);

template<typename Window>
static void noopDeleter(Window*){}

template<typename Window>
void ofSetupOpenGL(Window * windowPtr, int w, int h, ofWindowMode screenMode){
	std::shared_ptr<Window> window = std::shared_ptr<Window>(windowPtr, std::function<void(Window *)>(noopDeleter<Window>));
	ofSetupOpenGL(window,w,h,screenMode);
}


int ofRunApp(std::shared_ptr<ofBaseApp> && OFSA);
int ofRunApp(ofBaseApp * OFSA = nullptr); // will be deprecated
void ofRunApp(std::shared_ptr<ofAppBaseWindow> window, std::shared_ptr<ofBaseApp> && app);
int ofRunMainLoop();


ofBaseApp * ofGetAppPtr();

void		ofExit(int status=0);

//-------------------------- cursor
void 		ofHideCursor();
void 		ofShowCursor();
//-------------------------- window / screen
int 		ofGetWindowPositionX();
int 		ofGetWindowPositionY();
int 		ofGetScreenWidth();
int 		ofGetScreenHeight();
int			ofGetWindowMode();
int 		ofGetWidth();			// ofGetWidth is correct for orientation
int 		ofGetHeight();
int 		ofGetWindowWidth();			// ofGetWindowWidth is correct for actual window coordinates - so doesn't change with orientation.
int 		ofGetWindowHeight();

std::string ofGetClipboardString();
void		ofSetClipboardString(const std::string & str);

/// \returns a random number between 0 and the width of the window.
float ofRandomWidth();

/// \returns a random number between 0 and the height of the window.
float ofRandomHeight();
ImVec2	ofGetWindowSize();
ofRectangle	ofGetWindowRect();
ofAppBaseWindow * ofGetWindowPtr();
std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow();

void 		ofSetWindowPosition(int x, int y);
void 		ofSetWindowShape(int width, int height);
void 		ofSetWindowTitle(std::string title);
void		ofEnableSetupScreen();
void		ofDisableSetupScreen();
void		ofSetFullscreen(bool fullscreen);
void		ofToggleFullscreen();
//-------------------------- sync
void 		ofSetVerticalSync(bool bSync);

ofCoreEvents & ofEvents();
void ofSetCurrentRenderer(std::shared_ptr<ofBaseRenderer> renderer,bool setDefaults=false);
std::shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer();
void ofSetEscapeQuitsApp(bool bQuitOnEsc);