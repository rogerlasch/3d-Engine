@echo off
cl /nologo /EHsc /std:c++latest /c /MD /I "..\..\dependencies" *.cpp
link /nologo /LTCG /INCREMENTAL:NO /out:main.exe /LIBPATH:"..\..\dependencies\libs" /subsystem:"windows" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib windowlib.lib
del *.obj
