
@echo off

REM Verifica se a pasta build existe
IF NOT EXIST "build" (
    echo "Criando a pasta build..."
    mkdir build
)

REM Muda para a pasta build
cd build

REM Verifica se o argumento "debug" foi passado
if "%1"=="debug" (
    set MODE=debug
) else (
    set MODE=release
)

REM Remove arquivos de compilação antigos
IF EXIST *.exe (del *.exe)
IF EXIST *.obj (del *.obj)

REM Define as opções de compilação com base no modo selecionado
if "%MODE%"=="debug" (
    echo "Compilando em modo debug..."
    cl /MDd /Zi /nologo /EHsc /std:c++latest /I ..\..\ /I ..\dependencies /I D:\github\vcpkg\installed\x64-windows\include /c /MP ..\main.cpp
    link /nologo /INCREMENTAL:NO /DEBUG /out:"..\main.exe" /libpath:"..\..\dependencies\libs\debug" /LIBPATH:"D:\github\vcpkg\installed\x64-windows\debug\lib" /MACHINE:X64 /MANIFEST:NO *.obj gpp_d.lib gpp_physics_d.lib GameNetworkingSockets.lib enet.lib libcurl-d.lib
) else (
    echo "Compilando em modo release..."
    cl /MD /Zi /nologo /EHsc /std:c++latest /I ..\..\ /I ..\dependencies /I D:\github\vcpkg\installed\x64-windows\include /c /MP ..\main.cpp
    link /nologo /INCREMENTAL:NO /out:"..\main.exe" /libpath:"..\..\dependencies\libs" /LIBPATH:"D:\github\vcpkg\installed\x64-windows\lib" /MACHINE:X64 /MANIFEST:NO *.obj gpp.lib gpp_physics.lib GameNetworkingSockets.lib enet.lib libcurl.lib libcrypto.lib
)
cd ..
del *.exp
del *.lib
echo "Compilação concluída com sucesso!"
