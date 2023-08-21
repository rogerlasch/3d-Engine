@echo off
cl /nologo /EHsc /std:c++latest /c /MD /I "dependencies" *.cpp
link /nologo /LTCG /INCREMENTAL:NO /out:main.exe /LIBPATH:"dependencies\libs" /subsystem:"windows" /MACHINE:X86 /MANIFEST:NO *.obj windowlib.lib fmod_vc.lib
del *.obj
