@echo off
call :sub >z.txt
exit /b
:sub
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
mkdir build
cd build
cl /c /Od /MP /MT /Gw /Gm /ZI /nologo /EHsc /std:c++17 /I ..\..\gpp\dependencies ..\geometry\*.cpp ..\*.cpp ..\..\gpp\dependencies/fmt/*.cpp
lib /nologo /out:"..\bin/gpp_game.lib" /machine:X86 *.obj
