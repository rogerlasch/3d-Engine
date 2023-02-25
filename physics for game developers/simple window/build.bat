@echo off
cl /nologo /EHsc /std:c++latest /c /MD /I "..\..\dependencies" *.cpp ..\window\*.cpp
link /nologo /INCREMENTAL:NO /out:main.exe /LIBPATH:"..\..\dependencies\libs" /subsystem:"windows" /MACHINE:X86 /MANIFEST:NO *.obj SDL2.lib gpp.lib gpp_physics.lib
del *.obj
