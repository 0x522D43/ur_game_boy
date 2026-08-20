# Royal Game of Ur for Game Boy

A small Game Boy homebrew project inspired by the ancient Royal Game of Ur. The ROM is written in C and built with [GBDK-4](https://github.com/gbdk-2020/gbdk-2020). It includes a board, player tokens, a turn display, and a prototype dice and token movement flow.

The historical game is described in the [Royal Game of Ur article on Wikipedia](https://en.wikipedia.org/wiki/Royal_Game_of_Ur).

## Current status

This is a work-in-progress prototype. The current program provides the board and token interface, but it does not yet enforce every rule of the traditional game, including captures, blocked moves, exact winning conditions, or the full four-dice roll.

## Traditional game rules

The Royal Game of Ur is a race game for two players. Each player has seven pieces and tries to move all of them from their starting area, along the shared route, and off the board before the opponent.

- Each player rolls four binary tetrahedral dice. A roll produces a value from 0 to 4.
- A player moves one piece by the number rolled, following that player's route.
- The central part of the route is shared by both players; the outer lanes belong to only one player.
- A piece may not move onto a square occupied by another piece from the same player.
- Landing on an opponent's piece captures it and sends it back to that opponent's starting area. Pieces on rosette squares are protected from capture.
- Landing on a rosette grants an extra roll. The exact rosette layout can vary between reconstructed versions of the ancient board; this project uses its own board artwork.
- A player must roll a number that allows a piece to reach an open, valid square. If no legal move exists, the turn is passed.
- A piece must roll the exact number needed to leave the route. The first player to bear off all seven pieces wins.

These rules define the target gameplay for the finished project.

## Target result

The finished ROM should provide a complete two-player Royal Game of Ur experience on Game Boy:

- Display the full board, both players' seven tokens, the active player, and the current dice result.
- Generate a random result from four binary dice, producing a value from 0 to 4.
- Let the active player choose a token and show only legal moves for that roll.
- Move tokens along the correct player route, including the shared central route.
- Apply the traditional rosette rules: protected tokens cannot be captured, and landing on a rosette grants another roll.
- Capture an opponent's unprotected token when landing on its square and return it to its starting area.
- Pass the turn when the roll has no legal move.
- Prevent invalid moves, occupied friendly squares, and moves that overshoot the final square.
- Detect when a player has moved all seven tokens off the board and display a clear victory state.

The result should be playable with the Game Boy controls, build as `main.gb` inside the Dev Container, and run with Gearboy from the container terminal.

## Tools used

- **C** for the game code
- **GBDK-4** for Game Boy headers, graphics APIs, and the `lcc` compiler/linker wrapper
- **png2asset** for converting PNG images into Game Boy-compatible C tile and map data
- **GNU Make** for repeatable builds
- **Visual Studio Code** for editing and terminal-based development
- **Gearboy** for running and testing the generated `main.gb` ROM
- **Git** for source control

## Setup in Visual Studio Code

The project is designed for **Visual Studio Code Dev Containers**. A full Visual Studio solution is not required. The development container provides the command-line tools needed to build and run the project.

### 1. Install the host prerequisites

Install the following on the host machine:

1. Visual Studio Code
2. Docker Desktop, Docker Engine, or another compatible container runtime
3. The **Dev Containers** extension for Visual Studio Code
4. Git

GBDK-4, GNU Make, Gearboy, and the other development dependencies are provided through the project container features. No local Gearboy installation is required. The container is configured to run Gearboy from the VS Code terminal.

### 2. Reopen the project in the container

1. Open the project folder in Visual Studio Code.
2. Run **Dev Containers: Reopen in Container** from the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P`).
3. Wait for the container features and dependencies to finish installing.
4. Open **Terminal > New Terminal**. The terminal should be running inside the container at the project directory.

If Visual Studio Code does not offer the reopen command, install and enable the **Dev Containers** extension, then reload the window and try again. The first container start may take a few minutes while the image and features are downloaded.

### 3. Check the container toolchain

The Makefile expects `GBDKDIR` to be configured by the container features. From the bash-based VS Code terminal inside the container, check the variable and compiler:

```sh
echo "$GBDKDIR"
"$GBDKDIR/bin/lcc" -v
```

If `GBDKDIR` is empty, rebuild or reopen the container so its configured features and environment are applied before building.

### 4. Build the ROM

From the VS Code terminal, run:

```sh
make
```

This creates `main.gb`. To remove generated build files:

```sh
make clean
```

If VS Code detects the included tasks, the same commands are available from **Terminal > Run Task** as **make project**, **clean project**, and **remake project**.

### 5. Run the ROM

From the VS Code terminal inside the container, launch the ROM with Gearboy:

```sh
gearboy main.gb
```

Gearboy runs from the container, so do not install a separate local copy. Rebuild the ROM after changing C or asset files.

## Debug build

The Makefile supports optional GBDK debug output. Set `GBDK_DEBUG` before building:

```sh
GBDK_DEBUG=ON make
```

This passes debug and verbose flags to `lcc` and generates additional information used by the GBDK toolchain.

## License and attribution

The game concept is based on the ancient Royal Game of Ur. Historical rules and background information are linked above. Project asset and source licensing should be added here if this project is distributed publicly.
