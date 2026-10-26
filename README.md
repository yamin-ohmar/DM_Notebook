
# DM's Notebook

[![C++ Build and Test](https://github.com/yamin-ohmar/DM_Notebook/actions/workflows/ci.yml/badge.svg)](https://github.com/yamin-ohmar/DM_Notebook/actions/workflows/ci.yml)

A full-stack Dungeons & Dragons campaign management web application built using C++, Crow, SQLite and JavaScript.

The project aims to provide Dungeon Masters with a convenient way to organise campaign information, including NPCs, locations, quests and session notes.

**Project Status:** In active development.

## Features

### Implemented
- Create, view, edit and delete NPCs.
- Store NPC information, including name, race, role and description.
- Persist NPC records using SQLite.
- Responsive web interface displaying NPC cards.
- RESTful API for NPC management.
- Automated SQLite integration tests using CTest.
- GitHub Actions CI for automatic builds and testing.

### Planned
- Campaign management.
- Location and quest tracking.
- Session notes.
- Improved user interface and navigation.

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

### Build

```bash
git clone https://github.com/yamin-ohmar/DM_Notebook.git
cd DM_Notebook
cmake -S . -B build
cmake --build build
```

### Initialise the Database

Create the SQLite database and NPC table before launching the application:

```bash
sqlite3 database/dm_notebook.db < database/schema.sql
```

### Run

```bash
./build/DM_Notebook
```

Open http://localhost:18080 in your browser.

## Automated Testing

The project uses CTest to execute SQLite integration tests for NPC repository operations, including:

- Creating and retrieving NPCs.
- Updating existing NPCs.
- Deleting NPCs.
- Handling deletion of nonexistent NPCs.

Run the tests using:

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests use an isolated SQLite database to avoid modifying application data.

## Continuous Integration

GitHub Actions automatically builds and tests the project on Ubuntu whenever changes are pushed to `main` or a pull request targets `main`.

The CI workflow performs:

1. Dependency installation.
2. CMake configuration.
3. C++ compilation.
4. Automated CTest execution.

## Future Development

DM's Notebook is an ongoing learning project.

Planned improvements include:
- Campaign management functionality.
- Location and quest tracking.
- Session notes and campaign organisation.
- Improved frontend interface and navigation.
- Expanded automated test coverage.
- Automatic database initialisation.
