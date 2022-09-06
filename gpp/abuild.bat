@echo off
call :sub >z.txt
exit /b
:sub
del basscontroller.dll
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
cl /nologo /EHsc /std:c++17 /I dependencies /c /Gw /MT /MP *.cpp dependencies/fmt/*.cpp
link /nologo /INCREMENTAL:NO /out:main.exe /libpath:"dependencies\libs" /LTCG /MACHINE:X86 /MANIFEST:NO  *.obj GameNetworkingSockets.lib
del *.obj
