#ifndef GUI_APP_H
#define GUI_APP_H

#include "gui_rect.h"

namespace Gui
{
	struct App
	{
		App();
		~App();

		void Draw(const Rect& rect);

	protected:
        //--------------------------------------------------------------
        struct Context
        {
            void Create();
            void Draw(const Rect& screen);
            void Destroy();

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

            ImFontAtlas* imAtlas;

            void CreateGuiStyle();
            void CreateTexture();
            void CreateProgram();
            void CreateBuffers();
        } context;
        //--------------------------------------------------------------
	};
}
#endif 