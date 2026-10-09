# Battle Ramps — Homework Ninja Studios

**Produced by Homework Ninja Studios**

Dit is de broncode-repository voor het Battle Ramps Unreal Engine 5.8-starterproject. Het is geen kant-en-klare app: open de projectmap in Unreal Engine en bouw/package de game voor het besturingssysteem waarop je wilt spelen.

## Snel starten

1. Download de repository met **Code → Download ZIP**, of clone deze met Git.
2. Pak het bestand volledig uit.
3. Open `BattleRampsUE.uproject` in Unreal Engine 5.8.
4. Installeer Xcode op macOS of de benodigde Visual Studio C++-toolchain op Windows als Unreal daarom vraagt.
5. Laat Unreal de C++-modules bouwen en druk op **Play**.

## Wat zit erin?

- Grote procedureel opgebouwde stadionarena met basisbelichting.
- Achter-de-auto- en cockpitcamera (`V`).
- Vijf voertuigen: Rally GT, Neon Speedster, Battle Tank, Dune Buggy en Hyper X.
- Verschillende snelheid-, stuur- en HP-eigenschappen; de Battle Tank heeft het meeste leven.
- Teams van 1v1 tot 5v5: `O` vergroot en `P` verkleint het team.
- Bal met physics, doelen, scorebord en wedstrijdtimer van vijf minuten.
- Supply drops, beperkte bal-pulses en schansen.
- Wins, coins en cosmetische aankoop (`K`).
- Studiovermelding op het HUD.

## Besturing

| Toets | Actie |
|---|---|
| W/S of pijltjes | Gas / achteruit |
| A/D of pijltjes | Sturen |
| Left Shift | Turbo |
| Spatie | Bal-pulse (duwt de bal; het is nog geen echt projectielwapen) |
| J | Springen |
| B | Schans bouwen |
| V | Camera wisselen |
| 1–5 | Autotype kiezen |
| O / P | Teamgrootte omhoog / omlaag |
| K | Cosmetica kopen |
| Enter | Wedstrijd opnieuw starten |

## Belangrijke beperking

Dit project gebruikt voorlopig procedurele placeholder-geometrie. Het is een werkbasis om in Unreal verder te ontwikkelen, geen voltooide fotorealistische game. Voor fotorealisme zijn gelicentieerde 3D-auto's, PBR-texturen, stadionassets en uitgebreidere gameplay/physics nodig. Bekijk [Docs/REALISM_ASSETS_NL.md](Docs/REALISM_ASSETS_NL.md).

De code is toegevoegd aan GitHub, maar de volledige game is nog niet met Unreal Engine gecompileerd of op echte hardware interactief getest. Behandel dit dus als een ontwikkelstarter.
