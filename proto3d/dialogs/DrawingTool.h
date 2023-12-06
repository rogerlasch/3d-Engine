

#ifndef DRAWINGTOLL_H
#define DRAWINGTOOL_H

#include <wx/glcanvas.h>
#include <GL/gl.h>


class DrawingTool : public wxGLCanvas
{
private:
    wxGLContext* m_context;
public:
DrawingTool(wxWindow* parent, wxWindowID id = wxID_ANY, const int* attribList = 0);
virtual ~DrawingTool();
void OnPaint(wxPaintEvent& event);
void draw();
};
#endif
