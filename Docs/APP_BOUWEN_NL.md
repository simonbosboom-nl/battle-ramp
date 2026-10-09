# Van broncode naar echte Battle Ramps-app

**Produced by Homework Ninja Studios**

Deze repository is een Unreal Engine C++-starterproject. De buildscripts proberen op de computer waarop Unreal en de compiler zijn geïnstalleerd een zelfstandige build te maken.

## MacBook

1. Download deze repository via GitHub → Code → Download ZIP en pak het ZIP-bestand uit.
2. Installeer de Engine-versie die in `BattleRampsUE.uproject` staat via de Epic Games Launcher.
3. Installeer Xcode en accepteer de Xcode-licentie indien gevraagd.
4. Open Terminal in de uitgepakte repositorymap en voer uit:
   ```bash
   bash ./Build_Battle_Ramps_Mac.command
   ```
5. Als packaging slaagt, vind je de build in `BuildOutput/Mac`.

Als Unreal in een andere map is geïnstalleerd, stel dan eerst `UE_ROOT` in:
```bash
export UE_ROOT="/volledig/pad/naar/UE_5.8"
bash ./Build_Battle_Ramps_Mac.command
```

## Windows

1. Download en pak de repository uit.
2. Installeer Unreal Engine en de door die Engine-versie vereiste Visual Studio C++-toolchain.
3. Dubbelklik op `Build_Battle_Ramps_Windows.bat`.
4. Als packaging slaagt, vind je de build in `BuildOutput/Windows`.

## Grenzen

- De scripts installeren Unreal, Xcode of Visual Studio niet.
- Een Mac-build en een Windows-build moeten op hun respectieve platform (of compatibele buildomgeving) worden gemaakt.
- De scripts garanderen geen geslaagde build. De C++-broncode moet door de passende Unreal-versie worden gecompileerd en eventuele compilerfouten moeten worden opgelost.
- Er worden geen fotorealistische auto- of stadionassets meegeleverd. Zie `REALISM_ASSETS_NL.md`.

De repositorycode is voorbereid, maar is nog niet in Unreal Engine gecompileerd of interactief op echte hardware getest.
