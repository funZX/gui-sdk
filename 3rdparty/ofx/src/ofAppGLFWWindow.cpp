#include <algorithm>
#include <ofConstants.h>
#include "ofUtils.h"
#include "ofAppGLFWWindow.h"
#include "ofEvents.h"
#include "ofBaseApp.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofBaseRenderer.h"

using namespace std;

//-------------------------------------------------------
ofAppGLFWWindow::ofAppGLFWWindow()
:coreEvents(new ofCoreEvents) {
	buttonInUse			= 0;
	buttonPressed		= false;
	bWindowNeedsShowing	= true;

	targetWindowMode	= OF_WINDOW;

	ofAppPtr			= nullptr;

	pixelScreenCoordScale = 1;
	nFramesSinceWindowResized = 0;
	windowP = nullptr;
	windowW = 0;
	windowH = 0;
	currentW = 0;
	currentH = 0;

	glfwSetErrorCallback(error_cb);
}

ofAppGLFWWindow::~ofAppGLFWWindow(){
	close();
}

void ofAppGLFWWindow::close(){
	if(windowP){


		glfwSetMouseButtonCallback( windowP, nullptr );
		glfwSetCursorPosCallback( windowP, nullptr );
		glfwSetCursorEnterCallback( windowP, nullptr );
		glfwSetKeyCallback( windowP, nullptr );
		glfwSetWindowSizeCallback( windowP, nullptr );
		glfwSetFramebufferSizeCallback( windowP, nullptr);
		glfwSetWindowCloseCallback( windowP, nullptr );
		glfwSetScrollCallback( windowP, nullptr );
#if GLFW_VERSION_MAJOR>3 || GLFW_VERSION_MINOR>=1
		glfwSetDropCallback( windowP, nullptr );
#endif
		//hide the window before we destroy it stops a flicker on OS X on exit.
		glfwHideWindow(windowP);

		// We must ensure renderer is destroyed *before* glfw destroys the window in glfwDestroyWindow,
		// as `glfwDestroyWindow` at least on Windows has the effect of unloading OpenGL, making all
		// calls to OpenGL illegal.
		glRenderer.reset();

		glfwDestroyWindow(windowP);
		windowP = nullptr;
		events().disable();
		bWindowNeedsShowing = true;
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setup(const ofGLESWindowSettings & settings){
	const ofGLFWWindowSettings * glSettings = dynamic_cast<const ofGLFWWindowSettings*>(&settings);
	if(glSettings){
		setup(*glSettings);
	}else{
		setup(ofGLFWWindowSettings(settings));
	}
}

void ofAppGLFWWindow::setup(const ofGLFWWindowSettings & _settings){
	settings = _settings;

	if(!glfwInit( )){
		return;
	}

	glfwWindowHint(GLFW_RED_BITS, settings.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings.blueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, settings.alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.depthBits);
    glfwWindowHint(GLFW_STENCIL_BITS, settings.stencilBits);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, settings.numSamples);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
	glfwWindowHint(GLFW_DECORATED, settings.decorated);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glesVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glRenderer = std::make_shared<ofBaseRenderer>(this);

	GLFWwindow * sharedContext = nullptr;
	if(settings.shareContextWith){
		sharedContext = (GLFWwindow*)settings.shareContextWith->getWindowContext();
	}

    windowP = glfwCreateWindow(settings.getWidth(), settings.getHeight(), settings.title.c_str(), nullptr, sharedContext);
    if (!windowP) {
        std::cerr << "couldn't create GLFW window";
        return;
    }

    if (settings.windowMode == OF_FULLSCREEN) {
        int count = 0;
        auto monitors = glfwGetMonitors(&count);
        auto mode = glfwGetVideoMode(monitors[settings.monitor]);
        currentW = mode->width;
        currentH = mode->height;
        if (!settings.isPositionSet()) {
            if (count > 0) {
                int x = 0, y = 0;
                settings.monitor = ofClamp(settings.monitor, 0, count - 1);
                glfwGetMonitorPos(monitors[settings.monitor], &x, &y);
                settings.setPosition(ImVec2(x, y));
                setWindowPosition(settings.getPosition().x, settings.getPosition().y);
                auto mode = glfwGetVideoMode(monitors[settings.monitor]);
                setWindowPosition(settings.getPosition().x, settings.getPosition().y);
                currentW = mode->width;
                currentH = mode->height;
            }
        }
        else {
            setWindowPosition(settings.getPosition().x, settings.getPosition().y);
            currentW = settings.getWidth();
            currentH = settings.getHeight();
            for (int i = 0; i < count; i++) {
                int x = 0, y = 0;
                glfwGetMonitorPos(monitors[i], &x, &y);
                auto mode = glfwGetVideoMode(monitors[i]);
                int w = mode->width;
                int h = mode->height;
                ofRectangle rect(x - 1, y - 1, w + 1, h + 1);
                if (rect.inside(settings.getPosition())) {
                    currentW = mode->width;
                    currentH = mode->height;
                    break;
                }
            }
        }
        targetWindowMode = settings.windowMode;
        settings.windowMode = OF_WINDOW;
    }
    else {
        if (settings.isPositionSet()) {
            setWindowPosition(settings.getPosition().x, settings.getPosition().y);
        }
        glfwGetWindowSize(windowP, &currentW, &currentH);
    }

    if (settings.iconified) {
        iconify(true);
    }

	//don't try and show a window if its been requsted to be hidden
	bWindowNeedsShowing = settings.visible;

	glfwSetWindowUserPointer(windowP,this);

	windowW = settings.getWidth();
	windowH = settings.getHeight();

	glfwMakeContextCurrent(windowP);

	int framebufferW, framebufferH, tmpWindowW, tmpWindowH;
	glfwGetFramebufferSize(windowP, &framebufferW, &framebufferH);
	glfwGetWindowSize( windowP, &tmpWindowW, &tmpWindowH );

	//this lets us detect if the window is running in a retina mode
	if( framebufferW != tmpWindowW){
		pixelScreenCoordScale = (float)framebufferW / (float)tmpWindowW;
		if( pixelScreenCoordScale < 1 ){
			pixelScreenCoordScale = 1;
		}

		if(targetWindowMode == OF_WINDOW){
			auto position = getWindowPosition();
			setWindowShape(windowW, windowH);
			setWindowPosition(position.x, position.y);
		}
	}

	gladLoadGL(glfwGetProcAddress);
	glRenderer->setup();

	setVerticalSync(true);
	glfwSetMouseButtonCallback(windowP, mouse_cb);
	glfwSetCursorPosCallback(windowP, motion_cb);
	glfwSetCursorEnterCallback(windowP, entry_cb);
	glfwSetKeyCallback(windowP, keyboard_cb);
	glfwSetCharCallback(windowP, char_cb);
	glfwSetWindowSizeCallback(windowP, resize_cb);
	glfwSetFramebufferSizeCallback( windowP, framebuffer_size_cb);
	glfwSetWindowCloseCallback(windowP, exit_cb);
	glfwSetScrollCallback(windowP, scroll_cb);
#if GLFW_VERSION_MAJOR>3 || GLFW_VERSION_MINOR>=1
	glfwSetDropCallback( windowP, drop_cb );
#endif

}
//--------------------------------------------
ofCoreEvents & ofAppGLFWWindow::events(){
	return *coreEvents;
}

//--------------------------------------------
shared_ptr<ofBaseRenderer> & ofAppGLFWWindow::renderer(){
	return glRenderer;
}

//--------------------------------------------
void ofAppGLFWWindow::update(){
	events().notifyUpdate();

	//show the window right before the first draw call.
	if( bWindowNeedsShowing && windowP ){
		glfwShowWindow(windowP);
		bWindowNeedsShowing = false;
		if(targetWindowMode==OF_FULLSCREEN){
			setFullscreen(true);
		}
	}

}

//--------------------------------------------
void ofAppGLFWWindow::pollEvents(){
	glfwPollEvents();
}

//--------------------------------------------
void ofAppGLFWWindow::draw(){
	glRenderer->startRender();

	events().notifyDraw();

	glRenderer->finishRender();

	swapBuffers();
	nFramesSinceWindowResized++;
}

//--------------------------------------------
void ofAppGLFWWindow::swapBuffers() {
	glfwSwapBuffers(windowP);
}

//--------------------------------------------
void ofAppGLFWWindow::startRender() {
	renderer()->startRender();
}

//--------------------------------------------
void ofAppGLFWWindow::finishRender() {
	renderer()->finishRender();
}

//--------------------------------------------
bool ofAppGLFWWindow::getWindowShouldClose(){
	return glfwWindowShouldClose(windowP);
}

//--------------------------------------------
void ofAppGLFWWindow::setWindowShouldClose(){
	glfwSetWindowShouldClose(windowP,1);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowTitle(string title){
	settings.title = title;
	glfwSetWindowTitle(windowP,settings.title.c_str());
}

//------------------------------------------------------------
int ofAppGLFWWindow::getPixelScreenCoordScale(){
    return pixelScreenCoordScale;
}

//------------------------------------------------------------
ImVec2 ofAppGLFWWindow::getWindowSize(){
	return {currentW*pixelScreenCoordScale, currentH*pixelScreenCoordScale};
}

//------------------------------------------------------------
ImVec2 ofAppGLFWWindow::getWindowPosition(){
	int x, y;
	glfwGetWindowPos(windowP, &x, &y);
    
    x *= pixelScreenCoordScale;
    y *= pixelScreenCoordScale;

	return ImVec2(x,y);
}

//------------------------------------------------------------
int ofAppGLFWWindow::getCurrentMonitor(){
	int numberOfMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);

	int xW;	int yW;
	glfwGetWindowPos(windowP, &xW, &yW);

	for (int iC=0; iC < numberOfMonitors; iC++){
		int xM; int yM;
		glfwGetMonitorPos(monitors[iC], &xM, &yM);
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[iC]);
		ofRectangle monitorRect(xM, yM, desktopMode->width, desktopMode->height);
		bool bPointMatch = xW >= monitorRect.getMinX() && yW >= monitorRect.getMinY() && xW < monitorRect.getMaxX() && yW < monitorRect.getMaxY();
		//		if (monitorRect.inside(xW, yW)){
		if( bPointMatch ) {
			return iC;
			break;
		}
	}
	return 0;
}


//------------------------------------------------------------
ImVec2 ofAppGLFWWindow::getScreenSize(){
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(count>0){
		int currentMonitor = getCurrentMonitor();
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[currentMonitor]);
		if(desktopMode){
			return {desktopMode->width*pixelScreenCoordScale, desktopMode->height*pixelScreenCoordScale};
		}
	}
	return ImVec2();
}

//------------------------------------------------------------
int ofAppGLFWWindow::getWidth(){
	return currentW * pixelScreenCoordScale;
}

//------------------------------------------------------------
int ofAppGLFWWindow::getHeight(){
	return currentH * pixelScreenCoordScale;
}

//------------------------------------------------------------
GLFWwindow* ofAppGLFWWindow::getGLFWWindow(){
	return windowP;
}

//------------------------------------------------------------
ofWindowMode ofAppGLFWWindow::getWindowMode(){
	return settings.windowMode;
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowPosition(int x, int y){
	glfwSetWindowPos(windowP,x/pixelScreenCoordScale,y/pixelScreenCoordScale);
}

//------------------------------------------------------------
void ofAppGLFWWindow::setWindowShape(int w, int h){
	if(settings.windowMode == OF_WINDOW){
		windowW = w;
		windowH = h;
	}
	currentW = w/pixelScreenCoordScale;
	currentH = h/pixelScreenCoordScale;

	glfwSetWindowSize(windowP,currentW,currentH);
}

//------------------------------------------------------------
void ofAppGLFWWindow::hideCursor(){
	if(settings.windowMode == OF_FULLSCREEN){
		glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	}else{
		glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	}
};

//------------------------------------------------------------
void ofAppGLFWWindow::showCursor(){
	glfwSetInputMode(windowP,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
};

//------------------------------------------------------------
ofAppGLFWWindow * ofAppGLFWWindow::setCurrent(GLFWwindow* windowP){
	ofAppGLFWWindow * instance = static_cast<ofAppGLFWWindow *>(glfwGetWindowUserPointer(windowP));
	shared_ptr<ofMainLoop> mainLoop = ofGetMainLoop();
	if(mainLoop){
		mainLoop->setCurrentWindow(instance);
	}
	instance->makeCurrent();
	return instance;
}


namespace{
int glfwtToOFModifiers(int mods){
	int modifiers = 0;
	if(mods & GLFW_MOD_SHIFT){
		modifiers |= OF_KEY_SHIFT;
	}
	if(mods & GLFW_MOD_ALT){
		modifiers |= OF_KEY_ALT;
	}
	if(mods & GLFW_MOD_CONTROL){
		modifiers |= OF_KEY_CONTROL;
	}
	if(mods & GLFW_MOD_SUPER){
		modifiers |= OF_KEY_SUPER;
	}
	return modifiers;
}
}
//------------------------------------------------------------
void ofAppGLFWWindow::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);

	switch(button){
	case GLFW_MOUSE_BUTTON_LEFT:
		button = OF_MOUSE_BUTTON_LEFT;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		button = OF_MOUSE_BUTTON_RIGHT;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		button = OF_MOUSE_BUTTON_MIDDLE;
		break;
	}
	instance->buttonInUse = button;


	ofMouseEventArgs::Type action;
	if (state == GLFW_PRESS) {
		action = ofMouseEventArgs::Pressed;
		instance->buttonPressed=true;
	} else {
		action = ofMouseEventArgs::Released;
		instance->buttonPressed=false;
	}

	int modifiers = glfwtToOFModifiers(mods);

	ofMouseEventArgs args(action, instance->events().getMouseX(), instance->events().getMouseY(), button, modifiers);

	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::motion_cb(GLFWwindow* windowP_, double x, double y) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);

	ofMouseEventArgs::Type action;
	if(!instance->buttonPressed){
		action = ofMouseEventArgs::Moved;
	}else{
		action = ofMouseEventArgs::Dragged;
	}

	ofMouseEventArgs args(action,
	    x*instance->pixelScreenCoordScale,
	    y*instance->pixelScreenCoordScale,
	    instance->buttonInUse,
	    instance->events().getModifiers());
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::entry_cb(GLFWwindow *windowP_, int entered) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	ofMouseEventArgs::Type action;
	if(entered){
		action = ofMouseEventArgs::Entered;
	}else{
		action = ofMouseEventArgs::Exited;
	}

	ofMouseEventArgs args(action,
	    instance->events().getMouseX(),
	    instance->events().getMouseY(),
	    instance->buttonInUse,
	    instance->events().getModifiers());
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::scroll_cb(GLFWwindow* windowP_, double x, double y) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);

	ofMouseEventArgs args(ofMouseEventArgs::Scrolled,
	    instance->events().getMouseX(),
	    instance->events().getMouseY(),
	    instance->buttonInUse,
	    instance->events().getModifiers());
	args.scrollX = x;
	args.scrollY = y;
	instance->events().notifyMouseEvent(args);
}

//------------------------------------------------------------
void ofAppGLFWWindow::drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	ofDragInfo drag;
	drag.position = ImVec2(instance->events().getMouseX(), instance->events().getMouseY());
	drag.files.resize(numFiles);
	for(int i=0; i<(int)drag.files.size(); i++){
		drag.files[i] = dropString[i];
	}
	instance->events().notifyDragEvent(drag);
}

//------------------------------------------------------------
void ofAppGLFWWindow::error_cb(int errorCode, const char* errorDescription){
	std::cerr << errorCode << ": " << errorDescription;
}

//------------------------------------------------------------
void ofAppGLFWWindow::keyboard_cb(GLFWwindow* windowP_, int keycode, int scancode, int action, int mods) {
	int key = 0;
	uint32_t codepoint = 0;
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	switch (keycode) {
		case GLFW_KEY_ESCAPE:
			key = OF_KEY_ESC;
		    break;
		case GLFW_KEY_F1:
			key = OF_KEY_F1;
		    break;
		case GLFW_KEY_F2:
			key = OF_KEY_F2;
		    break;
		case GLFW_KEY_F3:
			key = OF_KEY_F3;
		    break;
		case GLFW_KEY_F4:
			key = OF_KEY_F4;
		    break;
		case GLFW_KEY_F5:
			key = OF_KEY_F5;
		    break;
		case GLFW_KEY_F6:
			key = OF_KEY_F6;
		    break;
		case GLFW_KEY_F7:
			key = OF_KEY_F7;
		    break;
		case GLFW_KEY_F8:
			key = OF_KEY_F8;
		    break;
		case GLFW_KEY_F9:
			key = OF_KEY_F9;
		    break;
		case GLFW_KEY_F10:
			key = OF_KEY_F10;
		    break;
		case GLFW_KEY_F11:
			key = OF_KEY_F11;
		    break;
		case GLFW_KEY_F12:
			key = OF_KEY_F12;
		    break;
		case GLFW_KEY_LEFT:
			key = OF_KEY_LEFT;
		    break;
		case GLFW_KEY_RIGHT:
			key = OF_KEY_RIGHT;
		    break;
		case GLFW_KEY_UP:
			key = OF_KEY_UP;
		    break;
		case GLFW_KEY_DOWN:
			key = OF_KEY_DOWN;
		    break;
		case GLFW_KEY_PAGE_UP:
			key = OF_KEY_PAGE_UP;
		    break;
		case GLFW_KEY_PAGE_DOWN:
			key = OF_KEY_PAGE_DOWN;
		    break;
		case GLFW_KEY_HOME:
			key = OF_KEY_HOME;
		    break;
		case GLFW_KEY_END:
			key = OF_KEY_END;
		    break;
		case GLFW_KEY_INSERT:
			key = OF_KEY_INSERT;
		    break;
		case GLFW_KEY_LEFT_SHIFT:
			key = OF_KEY_LEFT_SHIFT;
		    break;
		case GLFW_KEY_LEFT_CONTROL:
			key = OF_KEY_LEFT_CONTROL;
		    break;
		case GLFW_KEY_LEFT_ALT:
			key = OF_KEY_LEFT_ALT;
		    break;
		case GLFW_KEY_LEFT_SUPER:
			key = OF_KEY_LEFT_SUPER;
		    break;
		case GLFW_KEY_RIGHT_SHIFT:
			key = OF_KEY_RIGHT_SHIFT;
		    break;
		case GLFW_KEY_RIGHT_CONTROL:
			key = OF_KEY_RIGHT_CONTROL;
		    break;
		case GLFW_KEY_RIGHT_ALT:
			key = OF_KEY_RIGHT_ALT;
		    break;
		case GLFW_KEY_RIGHT_SUPER:
			key = OF_KEY_RIGHT_SUPER;
		    break;
		case GLFW_KEY_BACKSPACE:
			key = OF_KEY_BACKSPACE;
		    break;
		case GLFW_KEY_DELETE:
			key = OF_KEY_DEL;
		    break;
		case GLFW_KEY_ENTER:
			key = OF_KEY_RETURN;
			codepoint = '\n';
		    break;
		case GLFW_KEY_KP_ENTER:
			key = OF_KEY_RETURN;
			codepoint = '\n';
		    break;
		case GLFW_KEY_TAB:
			key = OF_KEY_TAB;
			codepoint = '\t';
		    break;
		case GLFW_KEY_KP_0:
			key = codepoint = '0';
		    break;
		case GLFW_KEY_KP_1:
			key = codepoint = '1';
		    break;
		case GLFW_KEY_KP_2:
			key = codepoint = '2';
		    break;
		case GLFW_KEY_KP_3:
			key = codepoint = '3';
		    break;
		case GLFW_KEY_KP_4:
			key = codepoint = '4';
		    break;
		case GLFW_KEY_KP_5:
			key = codepoint = '5';
		    break;
		case GLFW_KEY_KP_6:
			key = codepoint = '6';
		    break;
		case GLFW_KEY_KP_7:
			key = codepoint = '7';
		    break;
		case GLFW_KEY_KP_8:
			key = codepoint = '8';
		    break;
		case GLFW_KEY_KP_9:
			key = codepoint = '9';
		    break;
		case GLFW_KEY_KP_DIVIDE:
			key = codepoint = '/';
		    break;
		case GLFW_KEY_KP_MULTIPLY:
			key = codepoint = '*';
		    break;
		case GLFW_KEY_KP_SUBTRACT:
			key = codepoint = '-';
		    break;
		case GLFW_KEY_KP_ADD:
			key = codepoint = '+';
		    break;
		case GLFW_KEY_KP_DECIMAL:
			key = codepoint = '.';
		    break;
		case GLFW_KEY_KP_EQUAL:
			key = codepoint = '=';
		    break;
		default:
			key = codepoint;
		    break;
	}

	int modifiers = glfwtToOFModifiers(mods);

	if(action == GLFW_PRESS){
		ofKeyEventArgs keyE(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint,modifiers);
		instance->events().notifyKeyEvent(keyE);
	}else if(action == GLFW_REPEAT){
		ofKeyEventArgs keyE(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint,modifiers);
		keyE.isRepeat = true;
		instance->events().notifyKeyEvent(keyE);
	}else if (action == GLFW_RELEASE){
		ofKeyEventArgs keyE(ofKeyEventArgs::Released,key,keycode,scancode,codepoint,modifiers);
		instance->events().notifyKeyEvent(keyE);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::char_cb(GLFWwindow* windowP_, uint32_t key){
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->events().charEvent.notify(key);
}

//------------------------------------------------------------
void ofAppGLFWWindow::resize_cb(GLFWwindow* windowP_, int w, int h) {
	ofAppGLFWWindow * instance = setCurrent(windowP_);

	// Detect if the window is running in a retina mode

	int framebufferW, framebufferH; // <- physical pixel extents
	glfwGetFramebufferSize(windowP_, &framebufferW, &framebufferH);

	int windowW, windowH; // <- screen coordinates, which may be scaled
	glfwGetWindowSize(windowP_, &windowW, &windowH);

	glViewport(0, 0, windowW, windowH);
	glScissor(0, 0, windowW, windowH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Find scale factor needed to transform from screen coordinates
	// to physical pixel coordinates
	instance->pixelScreenCoordScale = (float)framebufferW / (float)windowW;

	if(instance->settings.windowMode == OF_WINDOW){
		instance->windowW = framebufferW;
		instance->windowH = framebufferH;
	}

	instance->currentW = windowW;
	instance->currentH = windowH;
	instance->events().notifyWindowResized(framebufferW, framebufferH);
	instance->nFramesSinceWindowResized = 0;
}

//------------------------------------------------------------
void ofAppGLFWWindow::framebuffer_size_cb(GLFWwindow* windowP_, int w, int h){

}

//--------------------------------------------
void ofAppGLFWWindow::exit_cb(GLFWwindow* windowP_){
	ofAppGLFWWindow * instance = setCurrent(windowP_);
	instance->events().notifyExit();
}

//------------------------------------------------------------
void ofAppGLFWWindow::setVerticalSync(bool bVerticalSync){
	if(bVerticalSync){
		glfwSwapInterval( 1);
	}else{
		glfwSwapInterval(0);
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::setClipboardString(const string& text) {
	glfwSetClipboardString(ofAppGLFWWindow::windowP, text.c_str());
}

//------------------------------------------------------------
string ofAppGLFWWindow::getClipboardString() {
	const char* clipboard = glfwGetClipboardString(ofAppGLFWWindow::windowP);

	if (clipboard) {
		return clipboard;
	} else {
		return "";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listVideoModes(){
	glfwInit();
	int numModes;
	const GLFWvidmode * vidModes = glfwGetVideoModes(nullptr, &numModes );
	for(int i=0; i<numModes; i++){
		std::cout << vidModes[i].width << " x " << vidModes[i].height
		<< vidModes[i].redBits+vidModes[i].greenBits+vidModes[i].blueBits << "bit";
	}
}

//------------------------------------------------------------
void ofAppGLFWWindow::listMonitors(){
	glfwInit();
	int count;
	const auto monitors = glfwGetMonitors(&count);
	for(int i = 0; i<count; i++){
		auto monitor = monitors[i];
		int w,h,x,y;
		glfwGetMonitorPhysicalSize(monitor,&w,&h);
		glfwGetMonitorPos(monitor,&x,&y);
		std::cout << i << ": " << glfwGetMonitorName(monitor) << ", physical size: " << w << "x" << h << "mm at " << x << ", " << y;
	}
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowIconified(){
	return glfwGetWindowAttrib(windowP, GLFW_ICONIFIED);
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowActive(){
//	return glfwGetWindowParam(GLFW_ACTIVE);
	return true;
}

//------------------------------------------------------------
bool ofAppGLFWWindow::isWindowResizeable(){
	return !glfwGetWindowAttrib(windowP, GLFW_RESIZABLE);
}

//------------------------------------------------------------
void ofAppGLFWWindow::iconify(bool bIconify){
	if(bIconify)
		glfwIconifyWindow(windowP);
	else
		glfwRestoreWindow(windowP);
}



void ofAppGLFWWindow::makeCurrent(){
	glfwMakeContextCurrent(windowP);
}

