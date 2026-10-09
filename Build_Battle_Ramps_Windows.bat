@echo off
setlocal EnableExtensions
set "ROOT=%~dp0"
set "PROJECT=%ROOT%BattlerampsUE.uproject"
set "OUTPUT=%ROOT%BuildOutput\Windows"

if not exist "%PROJECT%" (
  echo ERROR: BattleRampsUE.uproject was not found.
  echo Extract the full repository ZIP and run this script from that folder.
  pause
  exit /b 1
)

if defined UE_ROOT if exist "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" goto found
if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\RunUAT.bat" (
  set "UE_ROOT=C:\Program Files\Epic Games\UE_5.8"
  goto found
)
echo Unreal Engine could not be found automatically.
echo Install the Engine version from the Epic Games Launcher.
echo For a custom install location, define UE_ROOT first.
pause
exit /b 1

:found
echo BATTLE RAMPS - HOMEWORK NINJA STUDIOS
echo Engine: %UE_ROOT%
echo Output folder: %OUTPUT%
if not exist "%OUTPUT%" mkdir "%OUTPUT%"
call "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT%" -noP4 -platform=Win64 -clientconfig=Shipping -build -cook -stage -pak -archive -archivedirectory="%OUTPUT%" -utf8output
if errorlevel 1 (
  echo.
  echo PACKAGING FAILED. Read the first error messages above.
  echo Common causes: Unreal/Compiler is missing or C++ errors are present.
  pause
  exit /b 1
)
echo.
echo Build completed. Look in: %OUTPUT%
pause
