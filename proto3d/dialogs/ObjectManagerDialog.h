
#ifndef OBJECTMANAGERDIALOG_H
#define OBJECTMANAGERDIALOG_H

#include <wx/listctrl.h>
#include<map>
#include<string>


struct PropertInfo{
std::string name;
std::string value;
std::string rgx;
PropertInfo(const std::string& name, const std::string& value, const std::string& rgx){
this->name=name;
this->value=value;
this->rgx=rgx;
}
};

class ObjectManagerDialog: public wxDialog {
std::map<int, PropertInfo> hprops;
public:
 ObjectManagerDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:

protected:
 // begin wxGlade: ObjectManagerDialog::attributes
 wxListView* lb_object;
 wxButton* button_OK;
 wxButton* button_CANCEL;
 // end wxGlade

 DECLARE_EVENT_TABLE();

public:
void drawProperties();
 virtual void editPropertie(wxListEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // OBJECTMANAGERDIALOG_H
