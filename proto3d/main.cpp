#include <wx/wx.h>
#include <wx/image.h>
#include "dialogs/gui.h"

using namespace std;


class Proto3dApp: public wxApp {
public:
virtual ~Proto3dApp();
 bool OnInit();
};

IMPLEMENT_APP(Proto3dApp)

Proto3dApp::~Proto3dApp()
{
profiler_dump("proto3d_profiler.txt");
}

bool Proto3dApp::OnInit()
{
 wxInitAllImageHandlers();
 ProtoFrame* Proto3d = new ProtoFrame(NULL, wxID_ANY, wxEmptyString);
 SetTopWindow(Proto3d);
string lg_name="proto3d_log.txt";
log_create(lg_name, LG_ALL);
log_set_default(lg_name);
 Proto3d->Show();
 return true;
}
