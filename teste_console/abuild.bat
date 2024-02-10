@echo off
IF EXIST *.exe( del *.exe)
IF EXIST *.obj (del *.obj)
cl /MD /Zi /nologo /EHsc /std:c++latest /I ..\ /I ..\dependencies /I D:\github\vcpkg\installed\x64-windows\include /c /MP *.cpp
link /nologo /INCREMENTAL:NO /out:"main.exe" /libpath:"..\dependencies\libs" /LIBPATH:"D:\github\vcpkg\installed\x64-windows\lib" /MACHINE:X64 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets.lib libcurl.lib
