@echo off
	echo Building RegExpCalc64...
"C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" LightMeter.sln /p:Configuration=Release /p:Platform=x64 /t:Rebuild /v:minimal
if %ERRORLEVEL% EQU 0 (
    echo Build succeeded!
) else (
    echo Build failed with error %ERRORLEVEL%
)
