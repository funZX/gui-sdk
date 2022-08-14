#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofRectangle.h"

class ofBaseApp;
class ofGLESRenderer;
struct GLFWwindow;
class ofCoreEvents;
template<typename T>
class ofPixels_;
typedef ofPixels_<unsigned char> ofPixels;

class ofGLFWWindowSettings: public ofGLESWindowSettings{
public:
	ofGLFWWindowSettings(){}
	ofGLFWWindowSettings(const ofGLESWindowSettings & settings)
	:ofGLESWindowSettings(settings){}

	int numSamples = 4;

	bool doubleBuffering = true;
	int redBits = 8;
	int greenBits = 8;
	int blueBits = 8;
	int alphaBits = 8;
	int depthBits = 24;
	int stencilBits = 0;
	bool visible = true;
	bool iconified = false;
	bool decorated = true;
	bool resizable = true;
	int monitor = 0;
	bool multiMonitorFullScreen = false;
	std::shared_ptr<ofAppBaseWindow> shareContextWith;
};

class ofAppGLFWWindow : public ofAppBaseGLESWindow{

public:

	ofAppGLFWWindow();
	~ofAppGLFWWindow();

	// Can't be copied, use shared_ptr
	ofAppGLFWWindow(ofAppGLFWWindow & w) = delete;
	ofAppGLFWWindow & operator=(ofAppGLFWWindow & w) = delete;

	static void loop(){};
	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return true; }
	static bool needsPolling(){ return true; }
	static void pollEvents();


    // this functions are only meant to be called from inside OF don't call them from your code
    using ofAppBaseWindow::setup;
	void setup(const ofGLESWindowSettings & settings);
	void setup(const ofGLFWWindowSettings & settings);
	void update();
	void draw();
	bool getWindowShouldClose();
	void setWindowShouldClose();

	void hideCursor();
	void showCursor();

	int getHeight() const;
	int getWidth() const;

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();
    
    GLFWwindow* getGLFWWindow();
    void * getWindowContext(){return getGLFWWindow();}
	ofGLFWWindowSettings getSettings(){ return settings; }

	ImVec2	getWindowSize();
	ImVec2	getScreenSize();
	ImVec2 	getWindowPosition();

	void setWindowTitle(std::string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);

	ofWindowMode	getWindowMode();
	void			setVerticalSync(bool bSync);

    void			setClipboardString(const std::string& text);
    std::string     getClipboardString();

    int				getPixelScreenCoordScale();

    void 			makeCurrent();
	void swapBuffers();
	void startRender();
	void finishRender();
	float getDeltaTime() { return deltaTime; }

	static void listVideoModes();
	static void listMonitors();
	bool isWindowIconified();
	bool isWindowActive();
	bool isWindowResizeable();
	void iconify(bool bIconify);

private:
	static ofAppGLFWWindow * setCurrent(GLFWwindow* windowP);
	static void 	mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void 	motion_cb(GLFWwindow* windowP_, double x, double y);
	static void 	entry_cb(GLFWwindow* windowP_, int entered);
	static void 	keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void 	char_cb(GLFWwindow* windowP_, uint32_t key);
	static void 	resize_cb(GLFWwindow* windowP_, int w, int h);
	static void 	framebuffer_size_cb(GLFWwindow* windowP_, int w, int h);
	static void 	exit_cb(GLFWwindow* windowP_);
	static void		scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void 	drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString);
	static void		error_cb(int errorCode, const char* errorDescription);

	void close();

	std::unique_ptr<ofCoreEvents> coreEvents;
	std::shared_ptr<ofBaseRenderer> glRenderer;
	ofGLFWWindowSettings settings;

	ofWindowMode	targetWindowMode;

	int				windowW, windowH;		/// Physical framebuffer pixels extents
	int				currentW, currentH;		/// Extents of the window client area, which may be scaled by pixelsScreenCoordScale to map to physical framebuffer pixels.
	float           pixelScreenCoordScale;  /// Scale factor from virtual operating-system defined client area extents (as seen in currentW, currentH) to physical framebuffer pixel coordinates (as seen in windowW, windowH).

	ofRectangle		windowRect;

	int				buttonInUse;
	bool			buttonPressed;

	int 			nFramesSinceWindowResized;
	bool			bWindowNeedsShowing;
	bool			needsResizeCheck = false; /// Just for RPI at this point

	GLFWwindow* 	windowP;
    GLFWcursor*		mouseCursors[ImGuiMouseCursor_COUNT] = {};

	int				getCurrentMonitor();

	uint64_t		currentTime = 0;
	uint64_t		updateTime  = 0;
	uint64_t		frameTime   = 0;
	float			deltaTime   = 0.0f;
	float			deltaHistory[11] = {0};

	void			updateDeltaTime();
	float			smoothDeltaTime(float deltaTime);
	ofBaseApp *		ofAppPtr;
};


//#endif
