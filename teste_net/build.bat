@echo off
IF "%1"=="debug" (
    echo Building in Debug mode
    set "cl_cmd=/nologo /Zi /EHsc /std:c++latest /c /MDd /I ..\dependencies cliente.cpp servidor.cpp"
    set "link_cmd_1=/nologo /INCREMENTAL:NO /out:cliente.exe /LIBPATH:..\dependencies\libs\debug /MACHINE:X86 /MANIFEST:NO cliente.obj gpp_d.lib libcurl-d.lib GameNetworkingSockets.lib"
    set "link_cmd_2=/nologo /INCREMENTAL:NO /out:servidor.exe /LIBPATH:..\dependencies\libs\debug /MACHINE:X86 /MANIFEST:NO servidor.obj gpp_d.lib libcurl-d.lib GameNetworkingSockets.lib"
) ELSE (
    echo Building in Release mode
    set "cl_cmd=/nologo /Zi /EHsc /std:c++latest /c /MD /I ..\dependencies cliente.cpp servidor.cpp"
    set "link_cmd_1=/LTCG /nologo /INCREMENTAL:NO /out:cliente.exe /LIBPATH:..\dependencies\libs /MACHINE:X86 /MANIFEST:NO cliente.obj gpp.lib libcurl.lib GameNetworkingSockets.lib"
    set "link_cmd_2=/LTCG /nologo /INCREMENTAL:NO /out:servidor.exe /LIBPATH:..\dependencies\libs /MACHINE:X86 /MANIFEST:NO servidor.obj gpp.lib libcurl.lib GameNetworkingSockets.lib"
)

IF EXIST *.exe del *.exe
IF EXIST *.obj del *.obj
IF EXIST *.txt del *.txt
cls

cl %cl_cmd%
link %link_cmd_1%
link %link_cmd_2%

IF EXIST *.obj del *.obj
