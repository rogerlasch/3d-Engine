


#ifndef GSPEAK_H
#define GSPEAK_H

#include<gtypes.h>
using namespace gpp;

void gspeak_init();
void gspeak_unload();
void gspeak(const std::string& str, bool interrupt);
void gspeak_w(const std::wstring& str, bool interrupt);
#define _speak(str, ...) gspeak(gpp::safe_format(str, __VA_ARGS__), false)
#define _speak_interrupt(str, ...) gspeak(gpp::safe_format(str, __VA_ARGS__), true)
#define _wspeak(str, ...) gspeak_w(gpp::safe_wformat(str, __VA_ARGS__), false)
#define _wspeak_interrupt(str, ...) gspeak_w(gpp::safe_wformat(str, __VA_ARGS__), true)
#endif
