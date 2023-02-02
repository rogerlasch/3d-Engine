@echo off
call :sub >szdebug.txt
exit /b
:sub
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
msbuild gpp_physics.vcxproj /p:Configuration=Release -nologo
