@echo off
call "D:\sistema\programas\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
msbuild gpp.sln /p:Configuration=Release -nologo -m
