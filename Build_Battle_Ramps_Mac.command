#!/bin/bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
PROJECT="$ROOT/BattleRampsUE.uproject"
OUTPUT="$ROOT/BuildOutput/Mac"

if [[ ! -f "$PROJECT" ]]; then
  echo "ERROR: BattleRampsUE.uproject was not found."
  echo "Extract the complete Battle Ramps repository ZIP and run this script from that folder."
  read -r -p "Press Enter to close..."
  exit 1
fi

find_engine() {
  if [[ -n "${UE_ROOT:-}" && -f "$UE_ROOT/Engine/Build/BatchFiles/RunUAT.sh" ]]; then
    echo "$UE_ROOT"
    return 0
  fi
  for candidate in \
    "/Users/Shared/Epic Games/UE_5.8" \
    "/Users/Shared/Epic Games/UE_5.7" \
    "/Applications/Epic Games/UE_5.8" \
    "/Applications/Epic Games/UE_5.7"; do
    if [[ -f "$candidate/Engine/Build/BatchFiles/RunUAT.sh" ]]; then
      echo "$candidate"
      return 0
    fi
  done
  return 1
}

if ! UE_ROOT_FOUND="$(find_engine)"; then
  cat <<'MSG'
Unreal Engine was not found automatically.

1. Install the project's Unreal Engine version with the Epic Games Launcher.
2. If it is installed in another location, open Terminal and run:
   export UE_ROOT="/full/path/to/UE_5.8"
   bash "./Build_Battle_Ramps_Mac.command"

This script packages the Mac game; it does not download Unreal Engine for you.
MSG
  read -r -p "Press Enter to close..."
  exit 1
fi

UAT="$UE_ROOT_FOUND/Engine/Build/BatchFiles/RunUAT.sh"
echo "Battle Ramps — Homework Ninja Studios"
echo "Engine: $UE_ROOT_FOUND"
echo "Output: $OUTPUT"
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
echo "Packaging finished. Look in: $OUTPUT"
read -r -p "Press Enter to close..."
