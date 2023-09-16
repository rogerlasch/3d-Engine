@echo off
cl /nologo /EHsc /std:c++latest /c /MD /I "dependencies" /I"..\dependencies" /I "..\..\3d-Engine" *.cpp
link /nologo /LTCG /INCREMENTAL:NO /out:main.exe /LIBPATH:"dependencies\libs" /LIBPATH:"..\dependencies\libs" /subsystem:"windows" /MACHINE:X86 /MANIFEST:NO *.obj windowlib.lib fmod_vc.lib gpp.lib gpp_physics.lib
del *.obj
