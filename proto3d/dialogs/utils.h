
#ifndef PT3_UTILS_H
#define PT3_UTILS_H

bool pt3_is_numeric(const std::string& str);
bool pt3_parseVector(const std::string& str, vector3d& vec);
void pt3_edit_vector(const vector3d& v, std::vector<wxTextCtrl*>& htext);
bool pt3_validate(std::vector<wxTextCtrl*>& htext, vector3d& vec);
bool pt3_validate_string(const std::string& str, const std::string& rgx);
#endif
