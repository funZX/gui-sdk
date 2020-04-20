#pragma once

#include "ofConstants.h"

class ofAppBaseWindow;
class ofGui;

class ofBaseRenderer {
public:
    ofBaseRenderer(const ofAppBaseWindow* _window);
    virtual ~ofBaseRenderer();

    /// \brief Starts using this renderer as the rendering surface.
    virtual void startRender();
    /// \brief Stop using this renderer as the rendering surface.
    virtual void finishRender();
    /// \brief Initialize this renderer state
    virtual void setup();
    /// \brief Deinitialize this renderer state
    virtual void reset();
    
    unsigned    createTexture(int w, int h, unsigned char* pixels);
    void        deleteTexture(unsigned texID);

protected:
    void        createProgram();
    void        createBuffers();

    const ofAppBaseWindow* window;
    std::shared_ptr<ofGui> gui;

    struct {
        unsigned texture;
        int program;

        struct {
            int projmat;
            int sampler;
        } uni;
    } effect;

    struct {
        unsigned source;
        unsigned elements;
    } vbo;
};