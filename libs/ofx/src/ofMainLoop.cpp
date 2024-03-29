/*
 * ofMainLoop.cpp
 *
 *  Created on: Oct 25, 2014
 *      Author: arturo
 */

#include "ofMainLoop.h"
#include "ofWindowSettings.h"
#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"
#include "ofAppGLFWWindow.h"

using namespace std;

ofMainLoop::ofMainLoop()
:bShouldClose(false)
,status(0)
,allowMultiWindow(true)
,escapeQuits(true){

}

ofMainLoop::~ofMainLoop() {
	exit();
}

shared_ptr<ofAppBaseWindow> ofMainLoop::createWindow(const ofWindowSettings & settings){

	shared_ptr<ofAppGLFWWindow> window = std::make_shared<ofAppGLFWWindow>();
	addWindow(window);
	window->setup(settings);
	return window;
}

void ofMainLoop::run(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> && app){
	windowsApps[window] = app;
	if(app){
		ofAddListener(window->events().setup,app.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().update,app.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().draw,app.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().exit,app.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().keyPressed,app.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().keyReleased,app.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseMoved,app.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseDragged,app.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mousePressed,app.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseReleased,app.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseScrolled,app.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseEntered,app.get(),&ofBaseApp::mouseEntered,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().mouseExited,app.get(),&ofBaseApp::mouseExited,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().fileDragEvent,app.get(),&ofBaseApp::fileDragged,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchCancelled,app.get(),&ofBaseApp::touchCancelled,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchDoubleTap,app.get(),&ofBaseApp::touchDoubleTap,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchDown,app.get(),&ofBaseApp::touchDown,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchMoved,app.get(),&ofBaseApp::touchMoved,OF_EVENT_ORDER_APP);
		ofAddListener(window->events().touchUp,app.get(),&ofBaseApp::touchUp,OF_EVENT_ORDER_APP);
	}
	currentWindow = window;
	window->makeCurrent();
	if(!windowLoop){
		window->events().notifySetup();
	}
}

void ofMainLoop::run(std::shared_ptr<ofBaseApp> && app){
	if(!windowsApps.empty()){
		run(windowsApps.begin()->first, std::move(app));
	}
}

int ofMainLoop::loop(){
	if(!windowLoop){
		while(!bShouldClose && !windowsApps.empty()){
			loopOnce();
			pollEvents();
		}
	}else{
		windowLoop();
	}
	return status;
}

void ofMainLoop::loopOnce(){
	if(bShouldClose) return;
	for(auto i = windowsApps.begin(); !windowsApps.empty() && i != windowsApps.end();){
		if(i->first->getWindowShouldClose()){
			auto window = i->first;
			windowsApps.erase(i++); ///< i now points at the window after the one which was just erased
			window->close();
		}else{
			currentWindow = i->first;
			i->first->makeCurrent();
			i->first->update();
			i->first->draw();
			i++; ///< continue to next window
		}
	}
	loopEvent.notify(this);
}

void ofMainLoop::pollEvents(){
	if(windowPollEvents){
		windowPollEvents();
	}
}

void ofMainLoop::exit(){
	exitEvent.notify(this);

	for(auto i: windowsApps){
		shared_ptr<ofAppBaseWindow> window = i.first;
		shared_ptr<ofBaseApp> app = i.second;
		
		if(window == nullptr) {
			continue;
		}
		if(app == nullptr) {
			continue;
		}

		ofEventArgs args;
		ofNotifyEvent(window->events().exit, args, this);

		ofRemoveListener(window->events().setup,app.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().update,app.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().draw,app.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().exit,app.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().keyPressed,app.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().keyReleased,app.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseMoved,app.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseDragged,app.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mousePressed,app.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseReleased,app.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseScrolled,app.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseEntered,app.get(),&ofBaseApp::mouseEntered,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().mouseExited,app.get(),&ofBaseApp::mouseExited,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().windowResized,app.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().messageEvent,app.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().fileDragEvent,app.get(),&ofBaseApp::fileDragged,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchCancelled,app.get(),&ofBaseApp::touchCancelled,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchDoubleTap,app.get(),&ofBaseApp::touchDoubleTap,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchDown,app.get(),&ofBaseApp::touchDown,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchMoved,app.get(),&ofBaseApp::touchMoved,OF_EVENT_ORDER_APP);
		ofRemoveListener(window->events().touchUp,app.get(),&ofBaseApp::touchUp,OF_EVENT_ORDER_APP);
	}


	// reset applications then windows
	// so events are present until the
	// end of the application
	for(auto & window_app: windowsApps){
		window_app.second.reset();
	}
	windowsApps.clear();
}

shared_ptr<ofAppBaseWindow> ofMainLoop::getCurrentWindow(){
	return currentWindow.lock();
}

void ofMainLoop::setCurrentWindow(shared_ptr<ofAppBaseWindow> window){
	currentWindow = window;
}

void ofMainLoop::setCurrentWindow(ofAppBaseWindow * window){
	if(currentWindow.lock().get() == window){
		return;
	}
	for(auto i: windowsApps){
		if(i.first.get() == window){
			currentWindow = i.first;
			break;
		}
	}
}

shared_ptr<ofBaseApp> ofMainLoop::getCurrentApp(){
	return windowsApps[currentWindow.lock()];
}

void ofMainLoop::shouldClose(int _status){
	for(auto i: windowsApps){
		i.first->setWindowShouldClose();
	}
	bShouldClose = true;
	status = _status;
}

void ofMainLoop::setEscapeQuitsLoop(bool quits){
	escapeQuits = quits;
}

void ofMainLoop::keyPressed(ofKeyEventArgs & key){
	if (key.key == OF_KEY_ESC && escapeQuits == true){				// "escape"
		shouldClose(0);
    }
}
