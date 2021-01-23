#pragma once
#include "ofConstants.h"


/// \brief Used to represent the available windowing modes for the application.
enum ofWindowMode{
	/// \brief A floating application window.
	OF_WINDOW 		= 0,
	/// \brief A fullscreen application window.
	OF_FULLSCREEN 	= 1,
};

class ofWindowSettings{
public:
	ofWindowSettings()
	:windowMode(OF_WINDOW)
	,width(1024)
	,height(768)
	,sizeSet(false)
	,position(0,0)
	,positionSet(false){}

	virtual ~ofWindowSettings(){};

	std::string title;
	ofWindowMode windowMode;

	void setPosition(const ImVec2 & position) {
		this->position = position;
		this->positionSet = true;
	}

	void setSize(int width, int height) {
		this->width = width;
		this->height = height;
		this->sizeSet = true;
	}

	bool isSizeSet() const {
		return sizeSet;
	}

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}

	const ImVec2 & getPosition() const {
		return position;
	}

	bool isPositionSet() const {
		return positionSet;
	}

protected:
	int width;
	int height;
	bool sizeSet;
	ImVec2 position;
	bool positionSet;
};

class ofGLESWindowSettings: public ofWindowSettings{
public:
	ofGLESWindowSettings()
	:glesVersion(2){}

	ofGLESWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings), glesVersion(2) {
        const ofGLESWindowSettings * glesSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
        if(glesSettings){
            glesVersion = glesSettings->glesVersion;
        }
    }

	virtual ~ofGLESWindowSettings(){};

	void setGLESVersion(int version){
		glesVersion = version;
	}

	int glesVersion;
};
