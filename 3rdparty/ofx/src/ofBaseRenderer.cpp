#include "ofAppBaseWindow.h"
#include "ofGui.h"
#include "ofBaseRenderer.h"

using namespace std;

ofBaseRenderer::ofBaseRenderer(const ofAppBaseWindow* _window) {
	window = _window;
}

ofBaseRenderer::~ofBaseRenderer() {
	
}

void ofBaseRenderer::startRender() {
	gui->update();
}

void ofBaseRenderer::finishRender() {
    
    gui->render();

    ImDrawData* draw_data = ImGui::GetDrawData();

    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho[4][4] =
    {
        { 2.0f / (R - L),       0.0f,                0.0f,   0.0f },
        { 0.0f,                 2.0f / (T - B),      0.0f,   0.0f },
        { 0.0f,                 0.0f,               -1.0f,   0.0f },
        { (R + L) / (L - R),    (T + B) / (B - T),   0.0f,   1.0f },
    };

    glUseProgram(effect.program);
    glUniform1i(effect.uni.sampler, 0);
    glUniformMatrix4fv(effect.uni.projmat, 1, GL_FALSE, &ortho[0][0]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            ImVec4 clip_rect;
            clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
            clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
            clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
            clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

            if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
            {
                int x = (int)(clip_rect.x);
                int y = (int)(fb_height - clip_rect.w);
                int w = (int)(clip_rect.z - clip_rect.x);
                int h = (int)(clip_rect.w - clip_rect.y);

                glScissor(x, y, w, h);

                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
            }
        }
    }
}

void ofBaseRenderer::setup() {
    ImVec2 windowSize = window->getWindowSize();

    ofResizeEventArgs evArgs;
    evArgs.width = windowSize.x;
    evArgs.height = windowSize.y;

    gui = std::make_shared<ofGui>(this);
    gui->setup();
    gui->onWindowResized(evArgs);


    memset(&effect, 0, sizeof(effect));
    memset(&vbo, 0, sizeof(vbo));

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

    createProgram();
    createBuffers();
}

void ofBaseRenderer::reset() {
    glDeleteBuffers(1, &vbo.elements);
    glDeleteBuffers(1, &vbo.source);
    glDeleteProgram(effect.program);

	gui->reset();
}

void ofBaseRenderer::createProgram() {
    const char* vsource =
        "attribute vec2 a_ScreenPosL;"
        "attribute vec2 a_TexCoord_0;"
        "attribute vec4 a_Color;"

        "uniform mat4 u_Matrix_Projection;"

        "varying vec2 v_Tex0;"
        "varying vec4 v_Color;"

        "void main()"
        "{"
        "	v_Tex0			= a_TexCoord_0;"
        "	v_Color			= a_Color;"

        "	gl_Position		= u_Matrix_Projection * vec4(a_ScreenPosL.xy, 0, 1);"
        "}";

    // ----------------------------------------------------------------------//

    const char* psource =
        "precision highp float;"

        "uniform lowp sampler2D	u_Sampler_Tex_0;"

        "varying vec2 v_Tex0;"
        "varying vec4 v_Color;"

        "void main()"
        "{"
        "	vec4 tex = texture2D( u_Sampler_Tex_0, v_Tex0.st );"
        "	gl_FragColor = v_Color * tex;"
        "}";

    int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vsource, nullptr);
    glCompileShader(vert);

    int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &psource, nullptr);
    glCompileShader(frag);

    effect.program = glCreateProgram();
    glAttachShader(effect.program, vert);
    glAttachShader(effect.program, frag);

    glBindAttribLocation(effect.program, 0, "a_ScreenPosL");
    glBindAttribLocation(effect.program, 1, "a_TexCoord_0");
    glBindAttribLocation(effect.program, 2, "a_Color");

    glLinkProgram(effect.program);

    effect.uni.projmat = glGetUniformLocation(effect.program, "u_Matrix_Projection");
    effect.uni.sampler = glGetUniformLocation(effect.program, "u_Sampler_Tex_0");

    glDeleteShader(frag);
    glDeleteShader(vert);
}

void ofBaseRenderer::createBuffers() {
    glGenBuffers(1, &vbo.source);
    glGenBuffers(1, &vbo.elements);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.source);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elements);
}

unsigned ofBaseRenderer::createTexture(int w, int h, unsigned char* pixels) {

    glGenTextures(1, &effect.texture);
    glBindTexture(GL_TEXTURE_2D, effect.texture);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return effect.texture;
}

void ofBaseRenderer::deleteTexture(unsigned texID) {
    glDeleteTextures(1, &texID);
}