
# DM's Notebook

[![C++ Build and Test](https://github.com/yamin-ohmar/DM_Notebook/actions/workflows/ci.yml/badge.svg)](https://github.com/yamin-ohmar/DM_Notebook/actions/workflows/ci.yml)

A lightweight, beginner-friendly Dungeons & Dragons campaign planning and storytelling web application built using C++, Crow, SQLite and JavaScript.

DM's Notebook aims to help Dungeon Masters organise campaigns, prepare sessions, record memorable adventures and maintain story continuity without requiring extensive knowledge of D&D rules.

**Project Status:** In active development.

## Features

### Implemented

**Campaign Management**
- Create, view, edit and delete multiple campaigns.
- Store campaign names and descriptions in SQLite.
- Display campaigns as cards on the homepage.
- RESTful API supporting campaign CRUD operations.

**NPC Management**
- Create, view, edit and delete NPCs.
- Store NPC names, races, roles and descriptions.
- Display NPCs as cards on a dedicated page.
- RESTful API supporting NPC CRUD operations.

**Backend and Testing**
- Modular C++ backend using models, repositories and routes.
- SQLite persistence using prepared statements and RAII.
- Automated SQLite integration tests for NPC and Campaign repositories using CTest.
- GitHub Actions CI for automated C++ builds and testing on Ubuntu.

### Planned

- Campaign dashboards and navigation.
- Campaign-specific NPCs and reusable resources.
- Session planning with objectives, story outlines and notes.
- Session outcomes, memorable moments and recaps.
- Player characters, locations, items, monsters and maps.
- Image support for campaign resources.
- Optional branching scenarios and DM scripts.
- Future AI-assisted session planning and narrative recaps.

## Tech Stack

| Component | Technologies |
|---|---|
| Backend | C++17, Crow |
| Frontend | HTML, CSS, JavaScript |
| Database | SQLite |
| Build System | CMake |
| Automated Testing | CTest |
| CI | GitHub Actions |
| Version Control | Git and GitHub |

## Getting Started

### Prerequisites

- C++17-compatible compiler
- CMake 3.20 or later
- Crow 1.3.4
- Asio
- SQLite3 development libraries
- SQLite3 command-line tool for database initialisation

### Build

```bash
git clone https://github.com/yamin-ohmar/DM_Notebook.git
cd DM_Notebook
cmake -S . -B build
cmake --build build
```

### Initialise the Database

Create the SQLite database and required tables before launching the application:

```bash
sqlite3 database/dm_notebook.db < database/schema.sql
```

This initialises the `campaigns` and `npcs` tables.

### Run

```bash
./build/DM_Notebook
```

Open http://localhost:18080 in your browser.

Current pages:
- `/` — Campaign management homepage.
- `/npcs` — NPC management page.

**Note:** NPC records are not yet associated with individual campaigns.

## Automated Testing

The project uses CTest to execute SQLite integration tests for both NPC and Campaign repositories.

Current test coverage includes:
- Retrieving records from an empty database.
- Creating records and verifying stored information.
- Updating records and verifying changes.
- Deleting existing records.
- Handling nonexistent records during update or deletion.

Run the tests using:

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests use separate SQLite databases to avoid modifying application data.

## Continuous Integration

GitHub Actions automatically builds and tests the project on Ubuntu whenever changes are pushed to `main` or a pull request targets `main`.

The CI workflow performs:

1. Dependency installation.
2. CMake configuration.
3. C++ compilation.
4. Automated CTest execution.

## Future Development

DM's Notebook is an ongoing software engineering learning and portfolio project.

The next development priorities are:

1. Introduce individual campaign dashboards.
2. Associate NPCs and other resources with campaigns.
3. Implement session creation and planning.
4. Record session outcomes and memorable events.
5. Support manual session recaps and campaign continuity.
6. Add images, maps and additional campaign resources.
7. Explore optional AI-assisted storytelling features.

The application prioritises simplicity, accessibility and storytelling over complex D&D rule management.
