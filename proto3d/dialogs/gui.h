


#ifndef PROTO_GUI_H
#define PROTO_GUI_H

#include<wx/wx.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include"../gpp/gpp.h"
#include"../gpp_physics/gpp_physics.h"
using namespace gpp;
#include"ids.h"
#include"utils.h"
#include"AmbienteEditorDialog.h"
#include"ObjectManagerDialog.h"
#include"DrawingTool.h"
#include"ProtoFrame.h"

#define _ALERT(msg, ...) wxMessageBox(safe_format(msg, __VA_ARGS__), "Info", wxOK | wxICON_INFORMATION)
#define _INPUTBOX(str, value) wxGetTextFromUser ( str, "Info", value).ToStdString()
#endif

