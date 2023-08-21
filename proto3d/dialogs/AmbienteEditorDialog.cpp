
#include <wx/wx.h>
#include"ids.h"
#include"ObjectManagerDialog.h"
#include "AmbienteEditorDialog.h"



AmbienteEditorDialog::AmbienteEditorDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
 wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
 SetTitle(wxT("Configurar Ambiente"));
 wxBoxSizer* sz_main = new wxBoxSizer(wxVERTICAL);
 nt1 = new wxNotebook(this, wxID_ANY);
 sz_main->Add(nt1, 1, wxEXPAND, 0);
 notebook_1_pane_1 = new wxPanel(nt1, wxID_ANY);
 nt1->AddPage(notebook_1_pane_1, wxT("Geral"));
 wxGridSizer* grd_geral = new wxGridSizer(3, 3, 0, 0);
 wxBoxSizer* sz_name = new wxBoxSizer(wxVERTICAL);
 grd_geral->Add(sz_name, 1, wxEXPAND, 0);
 wxStaticText* label_1 = new wxStaticText(notebook_1_pane_1, wxID_ANY, wxT("Nome"));
 sz_name->Add(label_1, 0, 0, 0);
 txt_name = new wxTextCtrl(notebook_1_pane_1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_CENTRE);
 txt_name->SetFocus();
 sz_name->Add(txt_name, 0, 0, 0);
 wxBoxSizer* sz_timestep = new wxBoxSizer(wxVERTICAL);
 grd_geral->Add(sz_timestep, 1, wxEXPAND, 0);
 wxStaticText* label_2 = new wxStaticText(notebook_1_pane_1, wxID_ANY, wxT("Passo de tempo, em Milisegundos"));
 sz_timestep->Add(label_2, 0, 0, 0);
 txt_timestep = new wxTextCtrl(notebook_1_pane_1, wxID_ANY, wxT("250"), wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_CENTRE);
 sz_timestep->Add(txt_timestep, 0, 0, 0);
 grd_geral->Add(0, 0, 0, 0, 0);
 wxBoxSizer* sz_min = new wxBoxSizer(wxVERTICAL);
 grd_geral->Add(sz_min, 1, wxEXPAND, 0);
 wxStaticText* label_3 = new wxStaticText(notebook_1_pane_1, wxID_ANY, wxT("Min: X; Y; Z"));
 sz_min->Add(label_3, 0, 0, 0);
 txt_min = new wxTextCtrl(notebook_1_pane_1, wxID_ANY, wxT("0;0;0"), wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_CENTRE);
 sz_min->Add(txt_min, 0, 0, 0);
 wxBoxSizer* sz_max = new wxBoxSizer(wxVERTICAL);
 grd_geral->Add(sz_max, 1, wxEXPAND, 0);
 wxStaticText* label_4 = new wxStaticText(notebook_1_pane_1, wxID_ANY, wxT("Max: X; Y; Z"));
 sz_max->Add(label_4, 0, 0, 0);
 txt_max = new wxTextCtrl(notebook_1_pane_1, wxID_ANY, wxT("0;0;0"), wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_CENTRE);
 sz_max->Add(txt_max, 0, 0, 0);
 grd_geral->Add(0, 0, 0, 0, 0);
 checkbox_1 = new wxCheckBox(notebook_1_pane_1, wxID_ANY, wxT("Habilitar Gravidade"));
 grd_geral->Add(checkbox_1, 0, 0, 0);
 wxBoxSizer* sz_gravity = new wxBoxSizer(wxVERTICAL);
 grd_geral->Add(sz_gravity, 1, wxEXPAND, 0);
 wxStaticText* label_5 = new wxStaticText(notebook_1_pane_1, wxID_ANY, wxT("Gravidade em M/S"));
 sz_gravity->Add(label_5, 0, 0, 0);
 txt_gravity = new wxTextCtrl(notebook_1_pane_1, wxID_ANY, wxT("-9.81"), wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_CENTRE);
 sz_gravity->Add(txt_gravity, 0, 0, 0);
 grd_geral->Add(0, 0, 0, 0, 0);
 notebook_1_pane_2 = new wxPanel(nt1, wxID_ANY);
 nt1->AddPage(notebook_1_pane_2, wxT("Objetos"));
 wxGridSizer* grd_objects = new wxGridSizer(3, 2, 0, 0);
 wxBoxSizer* sz_objects = new wxBoxSizer(wxVERTICAL);
 grd_objects->Add(sz_objects, 1, wxEXPAND, 0);
 wxStaticText* label_6 = new wxStaticText(notebook_1_pane_2, wxID_ANY, wxT("Objetos"));
 sz_objects->Add(label_6, 0, 0, 0);
 lb_objects = new wxListCtrl(notebook_1_pane_2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES);
 lb_objects->AppendColumn(wxT("ID"), wxLIST_FORMAT_LEFT, -1);
 lb_objects->AppendColumn(wxT("Nome"), wxLIST_FORMAT_LEFT, -1);
 lb_objects->AppendColumn(wxT("Forma"), wxLIST_FORMAT_LEFT, -1);
 lb_objects->AppendColumn(wxT("Posição"), wxLIST_FORMAT_LEFT, -1);
 sz_objects->Add(lb_objects, 1, wxEXPAND, 0);
 btn_add = new wxButton(notebook_1_pane_2, IDM_NEW, wxT("Adicionar"));
 grd_objects->Add(btn_add, 0, 0, 0);
 grd_objects->Add(0, 0, 0, 0, 0);
 btn_edit = new wxButton(notebook_1_pane_2, IDM_EDIT, wxT("Editar"));
 grd_objects->Add(btn_edit, 0, 0, 0);
 grd_objects->Add(0, 0, 0, 0, 0);
 btn_remove = new wxButton(notebook_1_pane_2, IDM_REMOVE, wxT("Remover"));
 grd_objects->Add(btn_remove, 0, 0, 0);
 
 // WARNING: Code for instance "sz_btns" of "wxStdDialogButtonSizer" not generated: no suitable writer found
 
 button_OK = new wxButton(this, wxID_OK, wxEmptyString);
 button_OK->SetDefault();
 button_CANCEL = new wxButton(this, wxID_CANCEL, wxEmptyString);
 
 notebook_1_pane_2->SetSizer(grd_objects);
 notebook_1_pane_1->SetSizer(grd_geral);
 SetSizer(sz_main);
 sz_main->Fit(this);
 SetAffirmativeId(button_OK->GetId());
 SetEscapeId(button_CANCEL->GetId());
 
 Layout();
 Centre();
}


BEGIN_EVENT_TABLE(AmbienteEditorDialog, wxDialog)
 // begin wxGlade: AmbienteEditorDialog::event_table
 EVT_BUTTON(IDM_NEW, AmbienteEditorDialog::checkButtonCallback)
 EVT_BUTTON(IDM_EDIT, AmbienteEditorDialog::checkButtonCallback)
 EVT_BUTTON(IDM_REMOVE, AmbienteEditorDialog::checkButtonCallback)
 // end wxGlade
END_EVENT_TABLE();


void AmbienteEditorDialog::checkButtonCallback(wxCommandEvent &event)  // wxGlade: AmbienteEditorDialog.<event_handler>
{
 event.Skip();
switch(event.GetId())
{
case IDM_NEW:
{
ObjectManagerDialog dg(this, 0, "");
dg.ShowModal();
break;
}
}
}


// wxGlade: add AmbienteEditorDialog event handlers
