# Model Design

## Overzicht

Dit model simuleert een agent-gebaseerd systeem op een grid met domeinen en sociale interacties. Agents bewegen met behulp van een A* algoritme over een 2D grid en worden weergegeven in een Qt 2.5D GUI.

## Componenten

### GridWorld

`src/GridWorld.cpp`

- Beheert het grid van cellen en domeinen.
- `CellType` bevat `Empty`, `Obstacle`, `Agent`.
- `Domain` bevat `Home`, `School`, `Friends`, `Leisure`, `Path`, `Public`.
- `initializeDomainLayers()` bepaalt de layout van de wereld.
- `isAccessible()` controleert of een positie bereikbaar is binnen de toegestane domeinen.
- `randomPositionForDomain()` kiest een doelpositie binnen een domein.

### Agent

`include/Agent.h` en `src/Agent.cpp`

- Agents hebben:
  - `id`
  - `age`
  - `position`
  - `goal`
  - `homeDomain`
  - `currentActivity`
  - `preferences`
  - `friends`
  - `usedMaterials`
  - `usesSubstances`
  - `resistance`
- `chooseActivity(hour, dayOfWeek)` beslist de volgende activiteit.
- `shouldUseSubstance()` bepaalt middelengebruik op basis van voorkeuren en resistentie.

### Pathfinder

`src/Pathfinder.cpp`

- A* zoekalgoritme voor grid-routeplanning.
- Gebruikt een Manhattan heuristiek.
- Routes mogen alleen lopen over toegestane domeinen.
- De open- en closed sets worden beheerd met priority queue en unordered_map.

### Simulation

`include/Simulation.h` en `src/Simulation.cpp`

- Start en stopt een simulatie-thread.
- `step()` voert een tijdstap uit:
  - update agents
  - evalueer vriendschappen
  - simuleer middelengebruik
  - update wereldstatus
  - verhoog het uur en dag
- `speed_` regelt de snelheid van de loop in milliseconden.
- `updateAgents()` plant doelen en vraagt een pad op.
- `evaluateFriendships()` maakt vrienden als agents op dezelfde positie komen.

### GUI

`src/MainWindow.cpp`

- Gebruikt Qt6 voor weergave.
- Tekent een isometrisch grid met kleurcodes per domein.
- Laat agents, domeinen en grenzen zien.
- Biedt knoppen voor `Start`, `Stop`, `Step`.
- Toont informatie over agents, tijd en vriendschappen.

## Belangrijke parameters en locaties

### Gridgrootte

- `src/MainWindow.cpp`: `world_(20, 20)`

### Domeinindeling

- `src/GridWorld.cpp`: `initializeDomainLayers()`
- Hier kun je kamers, paden en zones aanpassen.

### Agent initialisatie

- `src/MainWindow.cpp`: agenten toevoegen met `world_->addAgent(...)`
- Pas `Position`, `Domain`, `preferences`, `usesSubstances` en `resistance` aan.

### Obstakels

- `src/MainWindow.cpp`: `world_->setCell(x, y, CellType::Obstacle)`

### Simulatiesnelheid

- `src/Simulation.h`/`src/Simulation.cpp`: `speed_`
- Verlaag voor snellere updates, verhoog voor langzaam tempo.

### Gedrag en activiteiten

- `src/Agent.cpp`: `chooseActivity(...)`
- Hier kun je logica toevoegen voor schooldagen, vrije tijd en vriendenbezoek.

### Middelengebruik

- `src/Agent.cpp`: `shouldUseSubstance()`
- Pas de kans en voorwaarden aan voor meer realistische modellen.

## Modelontwerp principes

- De wereld is opgedeeld in domeinen die het gedrag sturen.
- Agents kiezen doelen op basis van hun huidige activiteit.
- Routes volgen alleen toegestane gebieden en vermijden obstakels.
- Vriendschap ontstaat door ruimte-tijd overlap.
- Middelengebruik is een sociaal gedrag dat voorkomt in leisure/friends domeinen.

## Aanpassingen voor Word en VS Code

- Open `README.md` en `docs/ModelDesign.md` in VS Code voor directe bewerking.
- Gebruik `pandoc` om de markdown om te zetten naar een Word-document:

```bash
brew install pandoc
pandoc docs/ModelDesign.md -o ModelDesign.docx
```

- Je kunt de markdown ook kopiëren naar Word of een Markdown-viewer gebruiken.
