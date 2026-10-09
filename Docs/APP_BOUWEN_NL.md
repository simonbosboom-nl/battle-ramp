# Van broncode naar echte Battle Ramps-app

**Produced by Homework Ninja Studios**

Deze repository bevat een Unreal Engine C++-starterproject. De scripts hieronder proberen een standalone build te maken op de computer waarop Unreal Engine en de benodigde compiler al zijn geïnstalleerd.

## MacBook

1. Download deze repository via GitHub → Code → Download ZIP en pak het archief uit.
2. Installeer de Engine-versie in `BattleRampsUE.uproject` via de Epic Games Launcher.
3. Installeer Xcode en accepteer de Xcode-licentie als de compiler daarom vraagt.
4. Open Finder en dubbelklik op `Build_Battle_Ramps_Mac.command`.
5. Na afloop staat de gecompileerde build, als de build slaagt, in `BuildOutput/Mac`.

Als macOS de scriptuitvoering blokkeert, open Terminal in de uitgepakte map en voer uit:
```bash
bash ./Build_Battle_Ramps_Mac.command
```

Voor een aangepaste Engine-installatiemap kun je vooraf `UE_ROOT` instellen:
```bash
export UE_ROOT="/volledig/pad/naar/UE_5.8"
bash ./Build_Battle_Ramps_Mac.command
```

## Windows

1. Download en pak de repository uit.
2. Installeer de project-Engine via Epic Games Launcher.
3. Installeer de Visual Studio C++-toolchain die door die Unreal-versie wordt vereist.
4. Dubbelklik op `Build_Battle_Ramps_Windows.bat`.
5. Na een geslaagde build staat de build in `BuildOutput/Windows`.

## Wat deze scripts niet doen

- Ze installeren Unreal Engine of Xcode/Visual Studio niet.
- Ze bouwen geen Mac-app vanuit Windows of een Windows-game vanuit macOS.
- Ze downloaden geen externe auto- of stadionassets.
- Ze garanderen geen geslaagde build: de C++-broncode moet met de passende Unreal Engine worden gecompileerd en eventuele compileerfouten moeten worden opgelost.
- Ze veranderen placeholder-assets niet in fotorealistische modellen.

## Projectnotitie

Deze starter is nog geen geteste commerciële game. Als het verpakken faalt, kopieer dan de eerste rode errorregels uit het buildvenster; dat helpt om de echte compileerfout te vinden.
