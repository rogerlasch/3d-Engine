@echo off
call :sub >z.txt
exit /b
:sub
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
mkdir build
cd build
cl /nologo /EHsc /std:c++latest /I ..\..\gpp\dependencies /c /Gw /MT /MP ..\*.cpp ..\..\gpp\dependencies/fmt/*.cpp
lib /nologo /out:"..\bin/geometry3d.lib" /machine:X86 *.obj
cd ..\bin
cl /nologo /EHsc /std:c++latest /I ..\..\gpp\dependencies /I "D:\github\vcpkg\installed\x86-windows\include" /c /Gw /MT /MP *.cpp
link /nologo /INCREMENTAL:NO /out:main.exe /LIBPATH:"..\..\gpp\dependencies\libs" /LIBPATH:"D:\github\vcpkg\installed\x86-windows\lib" /subsystem:"windows" /MACHINE:X86 /MANIFEST:NO *.obj SDL2.lib
del *.obj
cd ..
