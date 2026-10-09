@echo off
setlocal EnableExtensions
set "ROOT=%~dp0"
set "PROJECT=%ROOT%BattlerampsUE.uproject"
set "OUTPUT=%ROOT%BuildOutput\Windows"
if not exist "%PROJECT%" (
  echo ERROR: BattleRampsUE.uproject was not found.
  echo Extract the complete repository ZIP and run this script from that folder.
  pause
  exit /b 1
)

if defined UE_ROOT if exist "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" goto found
if exist "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\RunUAT.bat" (
  set "UE_ROOT=C:\Program Files\Epic Games\UE_5.8"
  goto found
)
if exist "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat" (
  set "UE_ROOT=C:\Program Files\Epic Games\UE_5.7"
  goto found
)
echo Unreal Engine was not found automatically.
echo Install the required Unreal Engine version with the Epic Games Launcher.
echo For a custom install location, define UE_ROOT to your Unreal Engine folder first.
pause
exit /b 1

:found
echo Battle Ramps - Homework Ninja Studios
echo Engine: %UE_ROOT%
echo Output: %OUTPUT%
if not exist "%OUTPUT%" mkdir "%OUTPUT%"
call "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT%" -noP4 -platform=Win64 -clientconfig=Shipping -build -cook -stage -pak -archive -archivedirectory="%OUTPUT%" -utf8output
if errorlevel 1 (
  echo.
  echo Packaging FAILED. Read the error lines above. Common causes are a missing C++ toolchain or Unreal version mismatch.
  pause
  exit /b 1
)
echo.
echo Packaging finished. Look in: %OUTPUT%
pause
