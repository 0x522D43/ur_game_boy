# Royal Game of Ur for Game Boy

A Game Boy homebrew implementation of the ancient Royal Game of Ur. The game is written in C with [GBDK-4](https://github.com/gbdk-2020/gbdk-2020) and currently contains a playable board, tokens, turn display, and prototype movement flow.

## Status

This is an early prototype with the Royal Game of Ur rules implemented. Additional game modes and presentation features are still planned.

## Build and run

The project is intended to run in the VS Code Dev Container, which provides GBDK-4, `png2asset`, GNU Make, and Gearboy.

```sh
make              # Build main.gb
gearboy main.gb   # Run the ROM
make clean        # Remove generated build files
```

The Makefile expects `GBDKDIR` to be set by the container. Reopen the project in the Dev Container if it is missing.

## Next steps

- Add sound effects and music.
- Add a GitHub Actions pipeline that builds and publishes `main.gb`.
- Keep generated asset `.c` and `.h` files out of Git. Generate them during the build with `png2asset` from the source PNG files.
- Add a main menu.
- Add game modes for VS CPU and two players.

## Credits

The game is based on the ancient Royal Game of Ur. See the [Wikipedia article](https://en.wikipedia.org/wiki/Royal_Game_of_Ur) for historical background and rules.
