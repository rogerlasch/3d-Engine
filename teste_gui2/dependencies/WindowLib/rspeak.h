


#ifndef RSPEAK_H
#define RSPEAK_H

namespace rgames
{

void rspeak_init();
void rspeak_unload();
void rspeak(const std::string& str, bool interrupt);
void rspeak_w(const std::wstring& str, bool interrupt);
#define _speak(str, ...) rspeak(std::vformat(str, std::make_format_args(__VA_ARGS__)), false)
#define _speak_interrupt(str, ...) rspeak(std::vformat(str, std::make_format_args(__VA_ARGS__)), true)
}
#endif
