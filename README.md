# Agent-Based Modeling System

Een C++-simulatie voor agent-gebaseerde modellen met grid-gebaseerde A* padplanning en 2.5D visualisatie.

## Wat dit project doet

- Visualiseert een grid-wereld met domeinen zoals home, leisure, path en public.
- Simuleert agents met leeftijd, voorkeuren, vrienden en middelengebruik.
- Gebruikt A* pathfinding om doelen binnen toegestane domeinen te bereiken.
- Ondersteunt een Qt6 GUI voor live weergave.

## Mac M2 / VS Code setup

### Benodigdheden

- Homebrew
- Xcode command line tools
- CMake 3.16+
- Qt6
- Catch2 (optioneel voor tests)
- VS Code met CMake Tools en C/C++ extensies

### Installatie met Homebrew

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install cmake qt6 catch2
```

### Open in VS Code

1. Open VS Code.
2. Kies `File > Open...` en selecteer de projectmap `/Users/youssefboulfiham/Desktop/agent-based modeling/session 2`.
3. Installeer de extensies:
   - CMake Tools
   - C/C++
   - Qt for C++ (optioneel)
4. Maak een `build` map in de projectroot of gebruik de bestaande `build`.

## Bouwen en draaien

### Terminal methode

```bash
cd '/Users/youssefboulfiham/Desktop/agent-based modeling/session 2'
mkdir -p build
cd build
cmake ..
make
./AgentBasedModeling
```

### VS Code methode

- Gebruik de `CMake Configure` en `Build` taken in de taakbalk.
- Of open een terminal in VS Code en voer dezelfde commando's uit.

## Waar je parameters kunt wijzigen

### 1. `src/MainWindow.cpp`

- `world_(20, 20)` — pas de gridbreedte en -hoogte aan.
- `world_->setCell(...)` — voeg vaste obstakels toe.
- `world_->addAgent(...)` — initialiseer agents met:
  - `id`
  - `age`
  - startpositie `Position(x, y)`
  - `Domain` startdomein
  - voorkeuren (`preferences`) per domein
  - `usesSubstances` (true/false)
  - `resistance` (weerstand tegen middelengebruik)

### 2. `src/GridWorld.cpp`

- `initializeDomainLayers()` — definieert de modelwereld en domeinen.
- `randomPositionForDomain(Domain domain)` — bepaalt doelposities per domein.
- `isAccessible(...)` — controleert of een positie binnen toegestane domeinen ligt.

### 3. `src/Agent.h` en `src/Agent.cpp`

- `Agent` model attributen:
  - leeftijd
  - startpositie
  - voorkeuren
  - vrienden
  - middelengebruik
  - resistentie
- `chooseActivity(...)` — bepaalt gedrag op basis van uur en dag.
- `shouldUseSubstance()` — bepaalt of een agent middelen gebruikt.
- `logMaterialUse(...)` — registreert materiaalgebruik.

### 4. `src/Simulation.cpp`

- `speed_` — simulatiesnelheid in milliseconden tussen stappen.
- `currentHour_` / `currentDayOfWeek_` — tijdsturing.
- `updateAgents()` — plant doelen en routes voor agents.
- `evaluateFriendships()` — maakt vrienden als agents samenkomen.
- `simulateSubstanceUse()` — voert middelengebruik uit in leisure/friends domeinen.

### 5. `src/Pathfinder.cpp`

- `findPath(...)` — A* implementatie met Manhattan heuristiek.
- `allowedDomains` — beperk de route naar toegestane domeinen.
- `heuristic(...)` — verandert de routeafstand.

## Modelontwerp

De modelarchitectuur bestaat uit:

- `GridWorld` — wereldmodel met cellen en domeinen.
- `Agent` — enkelvoudige agent met gedrag en relaties.
- `Pathfinder` — A* routering tussen start en doel.
- `Simulation` — tijdgestuurde updates en logica.
- `MainWindow` — GUI weergave en bediening.

### Domeinen

- `Home`
- `School`
- `Friends`
- `Leisure`
- `Path`
- `Public`

### Kernstromen

1. Agents krijgen een doel op basis van hun huidige activiteit.
2. De pathfinder zoekt een route binnen toegestane domeinen.
3. Agent beweegt stap voor stap over het grid.
4. Vriendschap ontstaat wanneer agents samen op een positie komen.
5. Middelengebruik wordt gelogd in sociale/leisure domeinen.

## Converteer naar Word

Als je deze documentatie in Word wilt gebruiken, kun je de Markdown openen in VS Code en kopiëren/plakken naar Word.

Voor een directe conversie heb je `pandoc` nodig:

```bash
brew install pandoc
pandoc README.md -o AgentBasedModeling.docx
```

## Extra tips

- Sla veranderingen op en voer opnieuw `cmake ..` uit als je header- of CMake-wijzigingen maakt.
- Gebruik `make clean` of verwijder de `build` map bij grote structurele wijzigingen.
- Test wijzigingen door eerst `cmake --build build` te draaien.
