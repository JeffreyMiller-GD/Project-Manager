# prom — Project Manager

**prom** is a lightweight CLI tool for scaffolding, configuring, building, running, updating, and packaging C++ projects that use CMake, Ninja, and CPack. It generates a ready-to-build project skeleton (with an optional Windows resource/icon file) and wraps the common CMake/Ninja/CPack workflow behind a handful of simple subcommands.

Current version: **1.01** (`PROJECT_MANAGER_VERSION`)

## What's New in 1.01

- **Order-independent argument parsing** — the command-line parsing logic was optimized to eliminate the fixed-order requirement. Options such as `-pj <path>`, `--debug` / `--release`, `--verbose`, and `--rc` can now be supplied in any order, and are consistently accepted (with or without the leading `--`) across all subcommands.
- **New `run` command** — `prom run` builds the project and then **automatically executes the produced binary** after the build finishes. Currently the executable name must still be specified manually via `-bin <executable_name>`; automatic detection of the built binary is planned for a future release.

## Features

- **Project scaffolding** (`new`) — creates a standard directory layout, a generated `CMakeLists.txt`, a starter `src/main.cpp`, and a Windows `app.rc` resource file (icon inclusion commented out by default, enable with `--rc`), then runs the initial CMake configure step.
- **Build** (`build`) — invokes `cmake --build` against the debug or release build directory, with optional verbose output.
- **Run** (`run`) — builds the project and then automatically executes the resulting binary.
- **Reconfigure** (`update`) — re-runs CMake configuration (`cmake -S ... -B ...`) for the debug or release build directory, useful after editing `CMakeLists.txt`.
- **Packaging** (`pack`) — runs `cpack` against the generated `CPackConfig.cmake` for the chosen build type.
- **Tool auto-detection** — automatically locates `cmake`, `ninja`, and `cpack` on first run (via `where`/`which`) and stores their paths in a `config.json` next to the executable. If a tool can't be found, prom prompts for the path interactively and self-repairs the config if it becomes invalid.
- Licensed under **GPL-3.0-or-later**; `show w` / `show c` print the warranty and redistribution conditions.

## Requirements

- CMake
- Ninja
- CPack (typically bundled with CMake)
- A C++23-capable compiler toolchain

## Generated Project Layout

Running `prom new -pj <path>` produces:

```
<path>/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
├── resources/
│   └── app.rc
└── out/
    ├── build/
    │   ├── x64-debug/
    │   └── x64-release/
    └── packages/
```

The generated `CMakeLists.txt` targets C++23 by default (configurable), and on Windows can optionally embed `app.rc` as the application icon — pass `--rc` (or `rc`) to `prom new` to enable it. The note printed by `prom new` explains how the icon file itself must be provided.

## Usage

```
prom <command> [options]
prom help
```

Since 1.01, options may be given in **any order** — the parser no longer requires a fixed positional layout. The project path is always supplied with `-pj <path>`, build modes are selected with `--debug` / `--release` (or their bare forms `debug` / `release`), and conflicting mode flags cannot be combined.

### `new` — create a new project

```
prom new -pj <project_path> [<build_mode>] [--rc]
```

- `<build_mode>`: `--debug` | `--release` | `--both` (default: `--debug`)
- `--rc` (or `rc`): enable the Windows resource file / application icon in the generated project
- Creates the project skeleton described above and runs an initial CMake configure for the selected build mode(s).

### `build` — build an existing project

```
prom build -pj <project_root_path> [<build_type>] [--verbose]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `--verbose` (or `verbose`): enable verbose build output
- Runs `cmake --build` against `out/build/x64-debug` or `out/build/x64-release`.

### `run` — build and run

```
prom run -pj <project_root_path> [<build_type>] [--verbose] -bin <executable_name>
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `-bin <executable_name>`: the name of the produced executable (e.g. `myapp.exe` on Windows) — **currently required to be specified manually**
- Builds the project for the selected build type, then automatically executes the binary from the corresponding `out/build/x64-*` directory.

### `update` — reconfigure an existing project

```
prom update -pj <project_root_path> [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- Re-runs the CMake configure step (e.g. after modifying `CMakeLists.txt`).

### `pack` — package a built project

```
prom pack -pj <project_root_path> [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--release`)
- Runs `cpack` using the build's `CPackConfig.cmake`, placing output in `out/packages`.

### `show` — license information

```
prom show w   # show warranty disclaimer
prom show c   # show redistribution conditions
```

### `--version` / `-v` / `--v`

Prints the current prom version (`1.01`).

### `help`

Prints a summary of all commands, including the new `run` syntax.

## Configuration

On first run, prom looks for `config.json` next to its executable. If missing, it attempts to auto-detect `cmake`, `ninja`, and `cpack` on your system `PATH`; any tool it can't find is requested interactively. The resolved paths are then saved to `config.json` for future runs. If `config.json` becomes invalid or a stored path no longer exists, prom automatically attempts to re-detect and repair it on the next run.

## License

Copyright (C) 2026 Yvhang Cai (Jeffrey Miller)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.