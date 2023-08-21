@echo off
IF EXIST *.exe( del *.exe)
IF EXIST *.obj (del *.obj)
cl /MD /nologo /EHsc /std:c++latest /I ..\ /I ..\dependencies /I D:\github\vcpkg\installed\x86-windows\include /c /MP *.cpp
link /nologo /INCREMENTAL:NO /out:"main.exe" /libpath:"..\dependencies\libs" /LIBPATH:"D:\github\vcpkg\installed\x86-windows\lib" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets_s.lib
