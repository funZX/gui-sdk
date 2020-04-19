#ifndef GUI_RECT_H
#define GUI_RECT_H

#include <imgui.h>

namespace Gui
{
// ----------------------------------------------------------------------//

// ----------------------------------------------------------------------//

struct Rect
{
// ----------------------------------------------------------------------//
    Rect();
    Rect(ImVec2 pos, ImVec2 size);
    Rect(float x, float y, float width, float height);
    ~Rect();
// ----------------------------------------------------------------------//
    enum Align
    {
        InLeft,
        InRight,
        InTop,
        InBottom,

        OutLeft,
        OutRight,
        OutTop,
        OutBottom,

        HorCenter,
        VerCenter,
        Center
    };
// ----------------------------------------------------------------------//

    float                       Area() const;
    ImVec2                      Position() const;
    ImVec2                      Size() const;

    float						Left(void) const;
    float						Right(void) const;
    float						Top(void) const;
    float						Bottom(void) const;
    float						Width(void) const;
    float						Height(void) const;
    ImVec2                      TopLeft(void) const;
    ImVec2                      TopRight(void) const;
    ImVec2                      BottomLeft(void) const;
    ImVec2                      BottomRight(void) const;

    void						Move(ImVec2 d);
    void						Move(float x, float y);
    void						MoveTo(ImVec2 pos);
    void						MoveTo(float x, float y);

    void						Inflate(ImVec2 v);
    void						Inflate(float dw, float dh);
    void						Zoom(ImVec2 v);
    void						Zoom(float dw, float dh);
    void						Resize(ImVec2 v);
    void						Resize(float dw, float dh);
    void						Scale(ImVec2 v);
    void						Scale(float dw, float dh);

    void						SetCenter(ImVec2 center);
    void						SetCenter(float x, float y);
    ImVec2						GetCenter() const;

    void						AlignTo(Rect r, Align align);

    bool						IsInside(ImVec2 pos);
    bool						IsInside(float x, float y);

    void						Bound(float, float, float, float);
    void						Bound(ImVec2 pos, ImVec2 size);
    void						Bound(Rect);

    static Rect				Union(Rect, Rect);
    static Rect				Intersect(Rect, Rect);

// ----------------------------------------------------------------------//

protected:
    // ------------------------------------------------------------------//
    ImVec2						m_position;
    ImVec2						m_size;
    // ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace Gui

#endif
