
#include<string.h>
#include"gui.h"

using namespace std;


VectorDialog::VectorDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
 wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
 SetTitle(wxT("Vetor"));
 wxBoxSizer* sz_main = new wxBoxSizer(wxVERTICAL);
 wxBoxSizer* sz_vec = new wxBoxSizer(wxVERTICAL);
 sz_main->Add(sz_vec, 1, wxEXPAND, 0);
 wxBoxSizer* sz_x = new wxBoxSizer(wxVERTICAL);
 sz_vec->Add(sz_x, 1, wxEXPAND, 0);
 wxStaticText* label_1 = new wxStaticText(this, wxID_ANY, wxT("Coordenada X:"));
 sz_x->Add(label_1, 0, 0, 0);
 txt_x = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxHSCROLL);
 sz_x->Add(txt_x, 0, 0, 0);
 wxBoxSizer* sz_y = new wxBoxSizer(wxVERTICAL);
 sz_vec->Add(sz_y, 1, wxEXPAND, 0);
 wxStaticText* label_2 = new wxStaticText(this, wxID_ANY, wxT("Coordenada Y:"));
 sz_y->Add(label_2, 0, 0, 0);
 txt_y = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxHSCROLL);
 sz_y->Add(txt_y, 0, 0, 0);
 wxBoxSizer* sz_z = new wxBoxSizer(wxVERTICAL);
 sz_vec->Add(sz_z, 1, wxEXPAND, 0);
 wxStaticText* label_3 = new wxStaticText(this, wxID_ANY, wxT("Coordenada Z:"));
 sz_z->Add(label_3, 0, 0, 0);
 txt_z = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxHSCROLL);
 sz_z->Add(txt_z, 0, 0, 0);

 // WARNING: Code for instance "sz_btns" of "wxStdDialogButtonSizer" not generated: no suitable writer found

 button_OK = new wxButton(this, wxID_OK, wxEmptyString);
 button_OK->SetDefault();
 button_CANCEL = new wxButton(this, wxID_CANCEL, wxEmptyString);

 SetSizer(sz_main);
 sz_main->Fit(this);
 SetAffirmativeId(button_OK->GetId());
 SetEscapeId(button_CANCEL->GetId());

 Layout();
}


BEGIN_EVENT_TABLE(VectorDialog, wxDialog)
 // begin wxGlade: VectorDialog::event_table
 EVT_BUTTON(wxID_OK, VectorDialog::onValidate)
 // end wxGlade
END_EVENT_TABLE();

void VectorDialog::setVector(const vector3d& v)
{
this->vec=v;
txt_x->SetValue(to_string(vec.x));
txt_y->SetValue(to_string(vec.y));
txt_z->SetValue(to_string(vec.z));
}

vector3d VectorDialog::getVector()const
{
return  vec;
}

void VectorDialog::onValidate(wxCommandEvent &event)  // wxGlade: VectorDialog.<event_handler>
{
vector<wxTextCtrl*> arr={txt_x, txt_y, txt_z};
vector3d v;
if(pt3_validate(arr, v))
{
setVector(v);
 event.Skip();
}
else
{
_ALERT("Os valores estão em um formato inválido!");
}
}


// wxGlade: add VectorDialog event handlers
