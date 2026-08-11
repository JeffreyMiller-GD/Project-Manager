# prom — Project Manager

**prom** is a lightweight CLI tool for scaffolding, configuring, building, updating, and packaging C++ projects that use CMake, Ninja, and CPack. It generates a ready-to-build project skeleton (with an optional Windows resource/icon file) and wraps the common CMake/Ninja/CPack workflow behind a handful of simple subcommands.

Current version: **1.00** (`PROJECT_MANAGER_VERSION`)

## Features

- **Project scaffolding** (`new`) — creates a standard directory layout, a generated `CMakeLists.txt`, a starter `src/main.cpp`, and a Windows `app.rc` resource file (icon inclusion commented out by default), then runs the initial CMake configure step.
- **Build** (`build`) — invokes `cmake --build` against the debug or release build directory, with optional verbose output.
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

Running `prom new <path>` produces:

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

The generated `CMakeLists.txt` targets C++23 by default (configurable), and on Windows can optionally embed `app.rc` as the application icon — see the note printed by `prom new` for how to enable it.

## Usage

```
prom <command> ...
prom help
```

### `new` — create a new project

```
prom new <project_path> [<build_mode>]
```

- `<build_mode>`: `debug` | `release` | `both` (default: `debug`)
- Creates the project skeleton described above and runs an initial CMake configure for the selected build mode(s).

### `build` — build an existing project

```
prom build <project_root_path> [<build_type>] [verbose]
```

- `<build_type>`: `debug` | `release` (default: `debug`)
- Add `verbose` as the final argument for verbose build output.
- Runs `cmake --build` against `out/build/x64-debug` or `out/build/x64-release`.

### `update` — reconfigure an existing project

```
prom update <project_root_path> [<build_type>]
```

- `<build_type>`: `debug` | `release` (default: `debug`)
- Re-runs the CMake configure step (e.g. after modifying `CMakeLists.txt`).

### `pack` — package a built project

```
prom pack <project_root_path> [<build_type>]
```

- `<build_type>`: `debug` | `release` (default: `release`)
- Runs `cpack` using the build's `CPackConfig.cmake`, placing output in `out/packages`.

### `show` — license information

```
prom show w   # show warranty disclaimer
prom show c   # show redistribution conditions
```

### `--version` / `-v` / `--v`

Prints the current prom version (`1.00`).

### `help`

Prints a summary of all commands.

## Configuration

On first run, prom looks for `config.json` next to its executable. If missing, it attempts to auto-detect `cmake`, `ninja`, and `cpack` on your system `PATH`; any tool it can't find is requested interactively. The resolved paths are then saved to `config.json` for future runs. If `config.json` becomes invalid or a stored path no longer exists, prom automatically attempts to re-detect and repair it on the next run.

## License

Copyright (C) 2026 Yvhang Cai (Jeffrey Miller)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.