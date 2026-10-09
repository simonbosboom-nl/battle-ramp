# Realistische assets toevoegen

Dit project gebruikt voorlopig engine-primitieven als vervangbare placeholders. Een gewone afbeelding is geen 3D-model en assetlicenties verschillen per pakket.

## Checklist voor assets

- **Auto:** echte 3D Static Mesh met nette topology, aparte wielmesh indien nodig, PBR-materialen en correcte schaal in centimeters.
- **Materiaal:** Base Color, Normal, Roughness en waar nodig Metallic/AO; lak moet meestal glad en relatief weinig ruw zijn.
- **Gras:** tiling PBR-materialen en een passende schaal; een simpele groene kleur is geen realistisch gras.
- **Stadion:** tribunes, netten, doelen, led-borden, dakconstructie en detail op grote kijkafstanden.
- **Licentie:** controleer of de asset voor een game en distributie gebruikt mag worden.

Projectinstellingen voor Lumen, Lumen-reflecties, Virtual Shadow Maps, TSR en Nanite staan in Config/DefaultEngine.ini. Deze instellingen vervangen geen goede meshes, materialen of scene-inrichting.

## Auto's importeren

1. Download een auto-asset met een passende licentie van bijvoorbeeld [Fab](https://www.fab.com/).
2. Importeer de bestanden naar Unreal, bijvoorbeeld Content/BR/Art/Cars/.
3. Maak in de Content Browser een Blueprint Class die afstamt van BRCarPawn.
4. Open Blueprint Class Defaults → Art.
5. Vul ArtBodyMeshesByType met maximaal vijf car body meshes in de volgorde Rally GT, Neon Speedster, Battle Tank, Dune Buggy, Hyper X.
6. Vul ArtWheelMesh in wanneer de wielmesh apart is. Zet bArtModelIncludesWheels aan als de auto de banden al bevat.
7. Kies de Blueprint als CarPawnClass en Default Pawn Class in de GameMode-/projectinstellingen.

## Gras en stadion

1. Download gelicentieerde PBR-texturen voor gras en stadionmaterialen.
2. Importeer Base Color, Normal en Roughness en maak Unreal Materials.
3. Maak een Blueprint Class van BRArenaBuilder.
4. Stel GrassMaterial en StadiumMaterial in en wijs de Blueprint toe aan ArenaBuilderClass.

Voor commerciële kwaliteit zijn ook hoogwaardige autoverlichting, echte banden, reflecties, netten, stadiongelsuid, animaties en uitgebreide voertuigphysics nodig.

Officiële bronnen:
- Fab: https://www.fab.com/
- Unreal Engine C++-startgids: https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-cpp-quick-start
- Lumen-belichting en reflecties: https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-global-illumination-and-reflections-in-unreal-engine
