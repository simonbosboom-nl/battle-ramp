#!/bin/bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
PROJECT="$ROOT/BattleRampsUE.uproject"
OUTPUT="$ROOT/BuildOutput/Mac"

if [[ ! -f "$PROJECT" ]]; then
  echo "ERROR: BattleRampsUE.uproject was not found."
  echo "Extract the complete repository ZIP, then run this script from the repository folder."
  read -r -p "Press Enter to close..."
  exit 1
fi

find_engine() {
  if [[ -n "${UE_ROOT:-}" && -f "$UE_ROOT/Engine/Build/BatchFiles/RunUAT.sh" ]]; then
    printf '%s' "$UE_ROOT"
    return 0
  fi
  for candidate in \
    "/Users/Shared/Epic Games/UE_5.8" \
    "/Users/Shared/Epic Games/UE_5.7" \
    "/Applications/Epic Games/UE_5.8" \
    "/Applications/Epic Games/UE_5.8"; do
    if [[ -f "$candidate/Engine/Build/BatchFiles/RunUAT.sh" ]]; then
      printf '%s' "$candidate"
      return 0
    fi
  done
  return 1
}

if ! UE_FOUND="$(find_engine)"; then
  cat <<'MSG'
Unreal Engine could not be found automatically.
Install the Engine version specified in BattleRampsUE.uproject using the Epic Games Launcher.
If it is installed in another folder, open Terminal and run:
  export UE_ROOT="/full/path/to/UE_5.8"
  bash "./Build_Battle_Ramps_Mac.command"
This script packages the game; it does not install Unreal Engine.
MSG
  read -r -p "Press Enter to close..."
  exit 1
fi

UAT="$UE_FOUND/Engine/Build/BatchFiles/RunUAT.sh"
echo "BATTLE RAMPS — HOMEWORK NINJA STUDIOS"
echo "Unreal Engine: $UE_FOUND"
echo "Output folder: $OUTPUT"
mkdir -p "$OUTPUT"

bash "$UAT" BuildCookRun \
  -project="$PROJECT" \
  -noP4 \
  -platform=Mac \
  -clientconfig=Shipping \
  -build -cook -stage -pak -archive \
  -archivedirectory="$OUTPUT" \
  -utf8output

echo
echo "If packaging succeeded, your Mac build is in: $OUTPUT"
read -r -p "Press Enter to close..."
