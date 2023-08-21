#include"gui.h"

using namespace std;

ObjectManagerDialog::ObjectManagerDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
 wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
 SetTitle(wxT("Configurar Objeto"));
 wxBoxSizer* sz_main = new wxBoxSizer(wxVERTICAL);
 wxStaticText* label_1 = new wxStaticText(this, wxID_ANY, wxT("Objeto"));
 sz_main->Add(label_1, 0, 0, 0);
 lb_object = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES);
 lb_object->AppendColumn(wxT("Propriedade"), wxLIST_FORMAT_LEFT, -1);
 lb_object->AppendColumn(wxT("Valor"), wxLIST_FORMAT_LEFT, -1);
 sz_main->Add(lb_object, 1, wxEXPAND, 0);
 
 // WARNING: Code for instance "sz_btns" of "wxStdDialogButtonSizer" not generated: no suitable writer found
 
 button_OK = new wxButton(this, wxID_OK, wxEmptyString);
 button_OK->SetDefault();
 button_CANCEL = new wxButton(this, wxID_CANCEL, wxEmptyString);
 
 SetSizer(sz_main);
 sz_main->Fit(this);
 SetAffirmativeId(button_OK->GetId());
 SetEscapeId(button_CANCEL->GetId());

 Layout();
 Centre();
hprops.insert(make_pair(OBJ_ID, PropertInfo("Id", "0", "^[1-9]\\d*$")));
hprops.insert(make_pair(OBJ_NAME, PropertInfo("Nome", "Default", "^(?![_\\s])[a-zA-Z0-9\\s][a-zA-Z0-9_\\s]*[a-zA-Z0-9\\s](?![_\\s])$")));
hprops.insert(make_pair(OBJ_POSITION, PropertInfo("Posição", "0;0;0", "^-?\\d+(\\.\\d+)?;-?\\d+(\\.\\d+)?;-?\\d+(\\.\\d+)?;?$")));
hprops.insert(make_pair(OBJ_VEL, PropertInfo("Velocidade", "0;0;0", "^-?\\d+(\\.\\d+)?;-?\\d+(\\.\\d+)?;-?\\d+(\\.\\d+)?;?$")));
hprops.insert(make_pair(OBJ_MASS, PropertInfo("Massa", "0.0", "^(0\\.\\d+|[1-9]\\d*\\.\\d+|[1-9]\\d*|0)$")));
hprops.insert(make_pair(OBJ_RESTITUTION, PropertInfo("Restituição", "0.0", "^(0\\.\\d+|[1-9]\\d*\\.\\d+|[1-9]\\d*|0)$")));
drawProperties();
}


BEGIN_EVENT_TABLE(ObjectManagerDialog, wxDialog)
 // begin wxGlade: ObjectManagerDialog::event_table
 EVT_LIST_ITEM_ACTIVATED(wxID_ANY, ObjectManagerDialog::editPropertie)
 // end wxGlade
END_EVENT_TABLE();

void ObjectManagerDialog::drawProperties()
{
lb_object->DeleteAllItems();
for(auto it=hprops.begin(); it!=hprops.end(); ++it)
{
int x=lb_object->InsertItem(lb_object->GetItemCount(), "txt");
lb_object->SetItem(x, 0, it->second.name);
lb_object->SetItem(x, 1, it->second.value);
}
}

void ObjectManagerDialog::editPropertie(wxListEvent &event)  // wxGlade: ObjectManagerDialog.<event_handler>
{
try {
 event.Skip();
int32 index=event.GetIndex();
_GINFO("index:{}", index);
auto it=hprops.find(index);
string txt=_INPUTBOX("Digite o novo valor...", it->second.value);
if(txt.size()>0){
if(pt3_validate_string(txt, it->second.rgx))
{
it->second.value=txt;
lb_object->SetItem(index, 1, txt);
}
else
{
_ALERT("O valor digitado é inválido!");
}
}
} catch(const exception& e){
_GINFO("{}", e.what());
}
}


// wxGlade: add ObjectManagerDialog event handlers
