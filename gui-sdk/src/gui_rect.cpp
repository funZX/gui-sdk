#include <gui_rect.h>

// ----------------------------------------------------------------------//
	
Gui::Rect::Rect()
{
    m_position = ImVec2(0, 0);
    m_size = ImVec2(0, 0);
}

// ----------------------------------------------------------------------//

Gui::Rect::Rect(ImVec2 pos, ImVec2 size)
{
    m_position.x = pos.x;
    m_position.y = pos.y;

    m_size.x = size.x;
    m_size.y = size.y;
}

// ----------------------------------------------------------------------//

Gui::Rect::Rect(float x, float y, float width, float height)
{
    m_position.x = x;
    m_position.y = y;

    m_size.x = width;
    m_size.y = height;
}

// ----------------------------------------------------------------------//

Gui::Rect::~Rect()
{

}

// ----------------------------------------------------------------------//

float Gui::Rect::Area() const
{
    return m_size.x * m_size.y;
}
// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::Position() const
{
    return m_position;
}
// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::Size() const
{
    return m_size;
}

// ----------------------------------------------------------------------//

float Gui::Rect::Left(void) const
{
    return m_position.x;
}

// ----------------------------------------------------------------------//

float Gui::Rect::Right(void) const
{
    return (m_position.x + m_size.x);
}

// ----------------------------------------------------------------------//

float Gui::Rect::Top(void) const
{
    return m_position.y;
}

// ----------------------------------------------------------------------//

float Gui::Rect::Bottom(void) const
{
    return (m_position.y + m_size.y);
}

// ----------------------------------------------------------------------//

float Gui::Rect::Width(void) const
{
    return m_size.x;
}

// ----------------------------------------------------------------------//

float Gui::Rect::Height(void) const
{
    return m_size.y;
}

// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::TopLeft(void) const
{
    return ImVec2(Left(), Top());
}

// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::TopRight(void) const
{
    return ImVec2(Right(), Top());
}

// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::BottomLeft(void) const
{
    return ImVec2(Left(), Bottom());
}

// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::BottomRight(void) const
{
    return ImVec2(Right(), Bottom());
}

// ----------------------------------------------------------------------//

void Gui::Rect::Bound(float x, float y, float width, float height)
{
    m_position.x = x;
    m_position.y = y;
    m_size.x = width;
    m_size.y = height;
}
// ----------------------------------------------------------------------//

void Gui::Rect::Bound(ImVec2 pos, ImVec2 size)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
    m_size.x = size.x;
    m_size.y = size.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Bound(Rect r)
{
    m_position.x = r.m_position.x;
    m_position.y = r.m_position.y;
    m_size.x = r.m_size.x;
    m_size.y = r.m_size.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Move(ImVec2 d)
{
    m_position.x += d.x;
    m_position.y += d.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}

// ----------------------------------------------------------------------//

void Gui::Rect::MoveTo(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::MoveTo(ImVec2 pos)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Inflate(ImVec2 v)
{
    m_size.x += v.x;
    m_size.y += v.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Inflate(float dw, float dh)
{
    m_size.x += dw;
    m_size.y += dh;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Zoom(ImVec2 v)
{
    m_size.x += v.x;
    m_size.y += v.y;
    m_position.x -= (v.x * 0.5f);
    m_position.y -= (v.y * 0.5f);
}

// ----------------------------------------------------------------------//

void Gui::Rect::Zoom(float dw, float dh)
{
    m_size.x += dw;
    m_size.y += dh;
    m_position.x -= (dw * 0.5f);
    m_position.y -= (dh * 0.5f);
}

// ----------------------------------------------------------------------//

void Gui::Rect::Resize(ImVec2 v)
{
    m_size.x = v.x;
    m_size.y = v.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Resize(float w, float h)
{
    m_size.x = w;
    m_size.y = h;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Scale(ImVec2 v)
{
    m_size.x *= v.x;
    m_size.y *= v.y;
}

// ----------------------------------------------------------------------//

void Gui::Rect::Scale(float kw, float kh)
{
    m_size.x *= kw;
    m_size.y *= kh;
}

// ----------------------------------------------------------------------//

Gui::Rect Gui::Rect::Union(Rect r1, Rect r2)
{
    Rect o;

    float x = r1.m_position.x > r2.m_position.x ? r2.m_position.x : r1.m_position.x;
    float y = r1.m_position.y > r2.m_position.y ? r2.m_position.y : r1.m_position.y;
    float r = r1.Right()  < r2.Right()  ? r2.Right()  : r1.Right();
    float b = r1.Bottom() < r2.Bottom() ? r2.Bottom() : r1.Bottom();

    o.Bound(x, y, r - x, b - y);

    return o;
}

// ----------------------------------------------------------------------//

Gui::Rect Gui::Rect::Intersect(Rect r1, Rect r2)
{
    Rect o;

    float x = r1.m_position.x > r2.m_position.x ? r1.m_position.x : r2.m_position.x;
    float y = r1.m_position.y > r2.m_position.y ? r1.m_position.y : r2.m_position.y;
    float r = r1.Right()  < r2.Right()  ? r1.Right()  : r2.Right();
    float b = r1.Bottom() < r2.Bottom() ? r1.Bottom() : r2.Bottom();

    o.Bound(x, y, r - x, b - y);

    return o;
}

// ----------------------------------------------------------------------//

void Gui::Rect::SetCenter(ImVec2 center)
{
    m_position.x = center.x - (m_size.x * 0.5f);
    m_position.y = center.y - (m_size.y * 0.5f);
}

// ----------------------------------------------------------------------//

void Gui::Rect::SetCenter(float x, float y)
{
    m_position.x = x - (m_size.x * 0.5f);
    m_position.y = y - (m_size.y * 0.5f);
}

// ----------------------------------------------------------------------//

ImVec2 Gui::Rect::GetCenter() const
{
    ImVec2 center;

    center.x = m_position.x + (m_size.x * 0.5f);
    center.y = m_position.y + (m_size.y * 0.5f);

    return center;
}

// ----------------------------------------------------------------------//

void Gui::Rect::AlignTo(Rect c, Align align)
{
    switch (align)
    {
    case Align::InRight:
        m_position.x = c.Right() - m_size.x;
        break;

    case Align::OutRight:
        m_position.x = c.Right();
        break;

    case Align::InLeft:
        m_position.x = c.m_position.x;
        break;

    case Align::OutLeft:
        m_position.x = c.m_position.x - m_size.x;
        break;

    case Align::InBottom:
        m_position.y = c.Bottom() - m_size.y;
        break;

    case Align::OutBottom:
        m_position.y = c.Bottom();
        break;

    case Align::InTop:
        m_position.y = c.Top();
        break;

    case Align::OutTop:
        m_position.y = c.Top() - Height();
        break;

    case Align::HorCenter:
        m_position.x = c.m_position.x + ((c.m_size.x - m_size.x) * 0.5f);
        break;

    case Align::VerCenter:
        m_position.y = c.m_position.y + ((c.m_size.y - m_size.y) * 0.5f);
        break;

    case Align::Center:
        m_position.x = c.m_position.x + ((c.m_size.x - m_size.x) * 0.5f);
        m_position.y = c.m_position.y + ((c.m_size.y - m_size.y) * 0.5f);
        break;
    }
}

// ----------------------------------------------------------------------//
bool Gui::Rect::IsInside(float x, float y)
{
    return (x > m_position.x && x < this->Right() && y > m_position.y && y < this->Top());
}

// ----------------------------------------------------------------------//

bool Gui::Rect::IsInside(ImVec2 v)
{
    return (v.x >= m_position.x && v.x < this->Right() && v.y >= m_position.y && v.y < this->Top());
}

// ----------------------------------------------------------------------//
