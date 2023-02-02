@echo off
call :sub >z.txt
exit /b
:sub
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
cl /nologo /EHsc /std:c++latest /I ..\ /I ..\dependencies /c /MT /MP *.cpp
link /nologo /INCREMENTAL:NO /out:"main.exe" /libpath:"..\dependencies\libs" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets_s.lib
