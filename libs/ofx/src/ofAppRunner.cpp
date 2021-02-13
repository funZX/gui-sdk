#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"
#include "ofMath.h"
#include "ofMainLoop.h"
#include "ofAppGLFWWindow.h"

using namespace std;


//--------------------------------------
void ofSetupOpenGL(shared_ptr<ofAppGLFWWindow> windowPtr, int w, int h, ofWindowMode screenMode){
	ofInit();
	auto settings = windowPtr->getSettings();
	settings.setSize(w,h);
	settings.windowMode = screenMode;
	ofGetMainLoop()->addWindow(windowPtr);
	windowPtr->setup(settings);
}

//--------------------------------------
namespace{
    shared_ptr<ofMainLoop> & mainLoop(){
        static shared_ptr<ofMainLoop> * mainLoop(new shared_ptr<ofMainLoop>(new ofMainLoop));
        return *mainLoop;
    }

    bool & initialized(){
        static bool * initialized = new bool(false);
        return *initialized;
    }

	bool & exiting(){
		static bool * exiting = new bool(false);
		return *exiting;
	}

	ofCoreEvents & noopEvents(){
		static auto * noopEvents = new ofCoreEvents();
		return *noopEvents;
	}
}



void ofExitCallback();
void ofURLFileLoaderShutdown();

void ofInit(){
	if(initialized()) return;
	initialized() = true;
	exiting() = false;
}

//--------------------------------------
shared_ptr<ofMainLoop> ofGetMainLoop(){
	return mainLoop();
}

//--------------------------------------
void ofSetMainLoop(shared_ptr<ofMainLoop> newMainLoop) {
	mainLoop() = newMainLoop;
}

//--------------------------------------
int ofRunApp(ofBaseApp * OFSA){
	mainLoop()->run(std::move(shared_ptr<ofBaseApp>(OFSA)));
	auto ret = ofRunMainLoop();
	ofExitCallback();
	return ret;
}

//--------------------------------------
int ofRunApp(shared_ptr<ofBaseApp> && app){
	mainLoop()->run(std::move(app));
	auto ret = ofRunMainLoop();
	ofExitCallback();
	return ret;
}

//--------------------------------------
void ofRunApp(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> && app){
	mainLoop()->run(window, std::move(app));
}

int ofRunMainLoop(){
	auto ret = mainLoop()->loop();
	return ret;
}

//--------------------------------------
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode){
	ofGLESWindowSettings settings;
	settings.setSize(w, h);
	settings.windowMode = screenMode;
	ofCreateWindow(settings);
}

shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings){
	ofInit();
	return mainLoop()->createWindow(settings);
}

//-----------------------	gets called when the app exits
//							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){
	if(!initialized()) return;

	// controlled destruction of the mainLoop before
	// any other deinitialization
	mainLoop()->exit();

	initialized() = false;
	exiting() = true;
}

//--------------------------------------
// core events instance & arguments
ofCoreEvents & ofEvents(){
	auto window = mainLoop()->getCurrentWindow();
	if(window){
		return window->events();
	}else{
		if(!exiting()){
            std::cerr << "Trying to call ofEvents() before a window has been setup";
            std::cerr << "We'll return a void events instance to avoid crashes but somethings might not work";
            std::cerr << "Set a breakpoint in " << __FILE__ << " line " << __LINE__ << " to check where is the wrong call";
		}
		return noopEvents();
	}
}

//--------------------------------------
void ofSetEscapeQuitsApp(bool bQuitOnEsc){
	mainLoop()->setEscapeQuitsLoop(bQuitOnEsc);
}

//--------------------------------------
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer(){
	return mainLoop()->getCurrentWindow()->renderer();
}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return mainLoop()->getCurrentApp().get();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return mainLoop()->getCurrentWindow().get();
}

//--------------------------------------
std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow() {
	return mainLoop()->getCurrentWindow();
}

//--------------------------------------
void ofExit(int status){
	mainLoop()->shouldClose(status);
}

//--------------------------------------
void ofHideCursor(){
	mainLoop()->getCurrentWindow()->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	mainLoop()->getCurrentWindow()->showCursor();
}

//--------------------------------------
void ofSetWindowPosition(int x, int y){
	mainLoop()->getCurrentWindow()->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	mainLoop()->getCurrentWindow()->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)mainLoop()->getCurrentWindow()->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)mainLoop()->getCurrentWindow()->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)mainLoop()->getCurrentWindow()->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)mainLoop()->getCurrentWindow()->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)mainLoop()->getCurrentWindow()->getWidth();
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)mainLoop()->getCurrentWindow()->getHeight();
}

//--------------------------------------------------
int ofGetWindowWidth(){
	return (int)mainLoop()->getCurrentWindow()->getWindowSize().x;
}
//--------------------------------------------------
int ofGetWindowHeight(){
	return (int)mainLoop()->getCurrentWindow()->getWindowSize().y;
}

//--------------------------------------------------
std::string ofGetClipboardString(){
	return mainLoop()->getCurrentWindow()->getClipboardString();
}

//--------------------------------------------------
void ofSetClipboardString(const std::string & str){
	mainLoop()->getCurrentWindow()->setClipboardString(str);
}

//--------------------------------------------------
ImVec2 ofGetWindowSize() {
	//this can't return ImVec2(ofGetWidth(), ofGetHeight()) as width and height change based on orientation.
	return mainLoop()->getCurrentWindow()->getWindowSize();
}

//--------------------------------------------------
float ofRandomWidth() {
	return ofRandom(0.f, ofGetWidth());
}

//--------------------------------------------------
float ofRandomHeight() {
	return ofRandom(0.f, ofGetHeight());
}

//--------------------------------------------------
ofRectangle	ofGetWindowRect() {
	return ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------
void ofSetWindowTitle(string title){
	mainLoop()->getCurrentWindow()->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	mainLoop()->getCurrentWindow()->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	mainLoop()->getCurrentWindow()->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	mainLoop()->getCurrentWindow()->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	mainLoop()->getCurrentWindow()->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return mainLoop()->getCurrentWindow()->getWindowMode();
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	mainLoop()->getCurrentWindow()->setVerticalSync(bSync);
}

