
#ifndef VECTORDIALOG_H
#define VECTORDIALOG_H

class VectorDialog: public wxDialog {
vector3d vec;
public:
 VectorDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:

protected:
 // begin wxGlade: VectorDialog::attributes
 wxTextCtrl* txt_x;
 wxTextCtrl* txt_y;
 wxTextCtrl* txt_z;
 wxButton* button_OK;
 wxButton* button_CANCEL;
 // end wxGlade

 DECLARE_EVENT_TABLE();

public:
void setVector(const vector3d& vec);
vector3d getVector()const;
 virtual void onValidate(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // VECTORDIALOG_H
