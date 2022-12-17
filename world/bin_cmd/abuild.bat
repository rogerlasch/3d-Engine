@echo off
del *.txt
del *.exe
cl /nologo /EHsc /Od /std:c++latest /I ..\..\gpp\dependencies /c /Gw /MT /MP *.cpp
link /nologo /INCREMENTAL:NO /out:main.exe /LIBPATH:"..\bin" /libpath:"..\..\gpp\dependencies\libs" /LIBPATH:"..\..\gpp\bin" /MACHINE:X86 /MANIFEST:NO *.obj gpp.lib gpp_game.lib GameNetworkingSockets.lib SDL2.lib Tolk.lib
del *.obj
call "main.exe"
