#include <glad/gl.h>
#include "gui_app.h"
#include "gui_wrap.h"
#include "virtual_console.h"

//--------------------------------------------------------------
void Gui::App::Context::Create()
{
    memset(&effect, 0, sizeof(effect));
    memset(&vbo, 0, sizeof(vbo));

    imAtlas = new ImFontAtlas;

    CreateTexture();
    CreateProgram();
    CreateBuffers();

    ImGui::CreateContext(imAtlas);
    CreateGuiStyle();

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
}
//--------------------------------------------------------------

void Gui::App::Context::Destroy()
{
    delete imAtlas;
    ImGui::DestroyContext();

    glDeleteBuffers(1, &vbo.elements);
    glDeleteBuffers(1, &vbo.source);
    glDeleteTextures(1, &effect.texture);
    glDeleteProgram(effect.program);
}

void Gui::App::Context::Draw(const Rect& screen)
{
    ImDrawData* draw_data = ImGui::GetDrawData();

    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
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

//--------------------------------------------------------------
void Gui::App::Context::CreateGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = ImVec4(0.72f, 0.84f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
}

//--------------------------------------------------------------

void Gui::App::Context::CreateTexture()
{
    const unsigned char ttf_font[] =
#                                   include "ttf_font.inl"

    ImFontConfig fontConfig;

    fontConfig.FontDataOwnedByAtlas = false;
    fontConfig.SizePixels = 13;
    fontConfig.FontDataSize = sizeof(ttf_font);

    imAtlas->AddFontFromMemoryTTF((void*)ttf_font, fontConfig.FontDataSize, fontConfig.SizePixels, &fontConfig);

    glGenTextures(1, &effect.texture);
    glBindTexture(GL_TEXTURE_2D, effect.texture);

    int texWidth = 0;
    int texHeight = 0;
    unsigned char* texBuf = nullptr;

    imAtlas->GetTexDataAsRGBA32(&texBuf, &texWidth, &texHeight);
    imAtlas->TexID = (void*)effect.texture;

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texWidth,
        texHeight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texBuf);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
//--------------------------------------------------------------

void Gui::App::Context::CreateProgram()
{
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
//--------------------------------------------------------------

void Gui::App::Context::CreateBuffers()
{
    glGenBuffers(1, &vbo.source);
    glGenBuffers(1, &vbo.elements);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.source);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elements);
}
//--------------------------------------------------------------

Gui::App::App()
{
    context.Create();
}
//--------------------------------------------------------------

Gui::App::~App()
{
    context.Destroy();
}
//--------------------------------------------------------------
void Gui::App::Draw(const Rect& rect)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = { rect.Width(), rect.Height() };
    ImGui::NewFrame();

    static Window w;

    w.MoveTo(rect.Position());
    w.Resize(rect.Size());

    Gui::BeginWindow(w, "Console", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    static Gui::Console console;
    console.Draw(w);

    Gui::EndWindow(w);

    float h = w.Height();
    w.MoveTo(0, h);
    w.Inflate(0, rect.Height() - h);

    Gui::BeginWindow(w, "MainWindow", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    static float k = 0;
    if (ImGui::SliderFloat("W", &k, 0, 100));
    Gui::EndWindow(w);

    ImGui::Render();

    context.Draw(rect);
}
//--------------------------------------------------------------