@echo off
IF EXIST *.exe( del *.exe)
IF EXIST *.obj (del *.obj)
IF EXIST *.txt (del *.txt)
cl /nologo /EHsc /std:c++latest /c /MD /I "..\dependencies" cliente.cpp
link /nologo /INCREMENTAL:NO /out:cliente.exe /LIBPATH:"..\dependencies\libs" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets.lib
del *.obj
cl /nologo /EHsc /std:c++latest /c /MD /I "..\dependencies" servidor.cpp
link /nologo /INCREMENTAL:NO /out:servidor.exe /LIBPATH:"..\dependencies\libs" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets.lib
del *.obj
IF EXIST *.obj (del *.obj)
