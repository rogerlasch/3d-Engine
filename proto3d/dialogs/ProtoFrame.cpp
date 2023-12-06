
#include"ids.h"
#include"gui.h"
#include "ProtoFrame.h"

ProtoFrame::ProtoFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
 wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
 SetSize(wxSize(800, 800));
 SetTitle(wxT("Proto3d"));
 ProtoMenu = new wxMenuBar();
 wxMenu *wxglade_tmp_menu;
 wxglade_tmp_menu = new wxMenu();
 wxglade_tmp_menu->Append(IDM_NEW, wxT("Novo"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_NEW);
 wxglade_tmp_menu->Append(IDM_SAVE, wxT("Salvar"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_SAVE);
 wxglade_tmp_menu->Append(IDM_LOAD, wxT("Carregar"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_LOAD);
 wxglade_tmp_menu->Append(IDM_EXIT, wxT("Sair"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_EXIT);
 ProtoMenu->Append(wxglade_tmp_menu, wxT("Arquivo"));
 wxglade_tmp_menu = new wxMenu();
 wxglade_tmp_menu->Append(IDM_PLAYPAUSE, wxT("Play&Pause"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_PLAYPAUSE);
 ProtoMenu->Append(wxglade_tmp_menu, wxT("Ações"));
 wxglade_tmp_menu = new wxMenu();
 wxglade_tmp_menu->Append(IDM_ABOUT, wxT("Sobre"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_ABOUT);
 wxglade_tmp_menu->Append(IDM_READ, wxT("Ler a documentação"), wxEmptyString);
 Bind(wxEVT_MENU, &ProtoFrame::checkMenuCallback, this, IDM_READ);
 ProtoMenu->Append(wxglade_tmp_menu, wxT("Ajuda"));
 SetMenuBar(ProtoMenu);
 /*
 panel_1 = new wxPanel(this, wxID_ANY);
 wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
 sizer_1->Add(0, 0, 0, 0, 0);

 panel_1->SetSizer(sizer_1);
 */
 wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
 mtool = new DrawingTool(this);
 sizer->Add(mtool, 1, wxEXPAND | wxALL, 0);
 SetSizer(sizer);

// Configurações iniciais OpenGL
 glClearColor(1.0f, 1.0f, 1.0, 1.0f);
this->Bind(wxEVT_PAINT, &DrawingTool::OnPaint, mtool);
 Layout();
}

BEGIN_EVENT_TABLE(ProtoFrame, wxFrame)
 // begin wxGlade: ProtoFrame::event_table
 // end wxGlade
END_EVENT_TABLE();


void ProtoFrame::checkMenuCallback(wxCommandEvent &event)  // wxGlade: ProtoFrame.<event_handler>
{
 event.Skip();
switch(event.GetId())
{
case IDM_NEW:
{
AmbienteEditorDialog dg(this, 0, "");
dg.ShowModal();
break;
}
case IDM_LOAD:
case IDM_SAVE:
break;
case IDM_EXIT:
this->Destroy();
break;
case IDM_PLAYPAUSE:
case IDM_ABOUT:
case IDM_READ:
break;
}
}


// wxGlade: add ProtoFrame event handlers
