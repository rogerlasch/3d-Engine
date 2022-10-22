@echo off
call :sub >z.txt
exit /b
:sub
del basscontroller.dll
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
mkdir build
cd build
cl /nologo /EHsc /std:c++latest /I ..\dependencies /c /Gw /MT /MP ..\*.cpp ..\dependencies/fmt/*.cpp
lib /nologo /out:"..\bin/gpp.lib" /machine:X86 *.obj
cd ..\bin
cl /nologo /EHsc /std:c++latest /I ..\dependencies /c /Gw /MT /MP cliente.cpp
link /nologo /INCREMENTAL:NO /out:cliente.exe /libpath:"..\dependencies\libs" /MACHINE:X86 /MANIFEST:NO cliente.obj gpp.lib GameNetworkingSockets.lib
cl /nologo /EHsc /std:c++17 /I ..\dependencies /c /Gw /MT /MP servidor.cpp
link /nologo /INCREMENTAL:NO /out:servidor.exe /libpath:"..\dependencies\libs" /MACHINE:X86 /MANIFEST:NO servidor.obj gpp.lib GameNetworkingSockets.lib
del *.obj
cd ..
