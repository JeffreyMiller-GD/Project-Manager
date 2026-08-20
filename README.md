# prom — Project Manager

**prom** is a lightweight CLI tool for scaffolding, configuring, building, running, updating, and packaging C++ projects that use CMake, Ninja, and CPack. It generates a ready-to-build project skeleton (with an optional Windows resource/icon file) and wraps the common CMake/Ninja/CPack workflow behind a handful of simple subcommands.

Current version: **1.04** (`PROJECT_MANAGER_VERSION`)

## What's New in 1.04

- **New per-project `configuration.json`** — the project's root-level `configuration.json` now specifies prom's defaults for that project:
  - `exe` — the default executable name (used by `run`).
  - `default_debug` — the default debug build directory.
  - `default_release` — the default release build directory.
  - `default_pkg` — the default packages output directory.
- **`pack`, `update`, `build`, and `run` now use `configuration.json`** — instead of the previously hard-coded `out/build/x64-debug`, `out/build/x64-release`, and `out/packages`, the debug/release build directories and the packages output directory are read from the project's `configuration.json`. Editing that file is enough to redirect every command to a different layout.
- **New `init` command** — `prom init -pj <project_root_path>` automatically generates a `configuration.json` in the project's root directory, pre-filled with the standard defaults (debug/release build directories, packages output directory, and the executable name set to the project folder name).
- **`new` generates `configuration.json` too** — every project created with `prom new` now ships with a ready-to-use `configuration.json`.
- **`run` resolves the executable name from `configuration.json`** — the `exe` value is used automatically, so the `-bin <executable_name>` option is no longer required; when `-bin` is supplied it takes precedence over the value stored in `configuration.json`.
- **Validation** — `build`, `run`, `update`, and `pack` now verify that `configuration.json` exists and contains the required keys. If it is missing or incomplete, prom reports the problem and tells you to run `init -pj <project_root_path>`.

## What's New in 1.03

- **Bug fixes** for regressions introduced with the Boost.Process rework in 1.02:
  - **Command error messages were not output** — stderr from child processes is now captured and printed.
  - **Error output stream was not correctly captured** — stderr runs through its own pipe (`std_err > ipstream`), drained by a dedicated thread, and forwarded to prom's own stderr.
  - **Forced debug build mode setting was incorrect** — build-mode selection when forcing `--debug` is corrected.
  - **Logs were buffered and output all at once at the end** — stdout/stderr pipes are now drained by dedicated threads while the child runs, so tool output streams to the console in real time instead of being flushed only after the process exits.
- **New `-b <build_path>` flag** (`build`, `run`, `update`, `pack`) — targets a specific build directory (the one containing `CMakeCache.txt`) instead of the default `out/build/x64-debug` / `out/build/x64-release`.
  - **Known behavior:** when `-b` is used, the build type recorded in `CMakeCache.txt` takes effect — the `--debug` / `--release` you pass is overridden (prom prints a warning about this when you use `-b`).
- **Also improved:** external tools are now invoked with an argument vector (no shell command-string parsing); `-pj` is optional for `build` / `run` / `update` / `pack` and defaults to the current working directory; configure/build/pack steps report a colored `[Succeed]-[code=N]` / `[Failed!]-[code=N]` status line.

## What's New in 1.02

- **No new features** — 1.02 was a maintenance release focused on how prom invokes external executables.
- **Boost.Process-based process invocation** — every external tool call (`cmake --build`, CMake configure, `cpack`, and the `where`/`which` tool discovery) now runs as a `boost::process` child process (v1 API) instead of a `std::system` shell command. Child output is captured through a pipe (`ipstream`) and streamed to the console line by line.
- **Tool auto-discovery without redirection** — locating `cmake`, `ninja`, and `cpack` no longer relies on shell redirection into a temporary output file; the discovery output is read directly from the process pipe (the old temp `output.txt` handling was removed).
- **Shell-injection hygiene** — moving away from `std::system` string commands eliminates the class of shell-injection issues associated with it, even though such attacks are virtually unheard of in this context.
- **No CLI or behavior changes** — commands, flags, and the `config.json` format are identical to 1.01.

## What's New in 1.01

- **Order-independent argument parsing** — the command-line parsing logic was optimized to eliminate the fixed-order requirement. Options such as `-pj <path>`, `--debug` / `--release`, `--verbose`, and `--rc` can now be supplied in any order, and are consistently accepted (with or without the leading `--`) across all subcommands.
- **New `run` command** — `prom run` builds the project and then **automatically executes the produced binary** after the build finishes. Since 1.04 the executable name is taken from the project's `configuration.json`; earlier releases required it to be specified manually via `-bin <executable_name>`.

## Features

- **Project scaffolding** (`new`) — creates a standard directory layout, a generated `CMakeLists.txt`, a starter `src/main.cpp`, a Windows `app.rc` resource file (icon inclusion commented out by default, enable with `--rc`), and a ready-to-use `configuration.json`, then runs the initial CMake configure step.
- **Project configuration** (`init`) — generates a `configuration.json` in the project's root directory, defining the default debug/release build directories, the packages output directory, and the default executable name used by `build` / `run` / `update` / `pack`.
- **Build** (`build`) — invokes `cmake --build` against the debug or release build directory (from `configuration.json`, or a custom `-b` build directory), with optional verbose output.
- **Run** (`run`) — builds the project and then automatically executes the resulting binary, using the executable name from `configuration.json`.
- **Reconfigure** (`update`) — re-runs CMake configuration (`cmake -S ... -B ...`) for the debug or release build directory (from `configuration.json`, or a custom `-b` build directory), useful after editing `CMakeLists.txt`.
- **Packaging** (`pack`) — runs `cpack` against the generated `CPackConfig.cmake` for the chosen build type (or a custom `-b` build directory), placing output in the packages directory from `configuration.json`.
- **Tool auto-detection** — automatically locates `cmake`, `ninja`, and `cpack` on first run (via `where`/`which`, executed as Boost.Process children) and stores their paths in a `config.json` next to the executable. If a tool can't be found, prom prompts for the path interactively and self-repairs the config if it becomes invalid.
- Licensed under **GPL-3.0-or-later**; `show w` / `show c` print the warranty and redistribution conditions.

## Requirements

- CMake
- Ninja
- CPack (typically bundled with CMake)
- A C++23-capable compiler toolchain
- **Boost** (build-time dependency since 1.02) — Boost.Process (v1 API, enabled via `BOOST_PROCESS_USE_V1`) and Boost.Filesystem, e.g. Boost 1.91. Point `BOOST_ROOT` / `BOOST_INCLUDEDIR` / `BOOST_LIBRARYDIR` in `CMakeLists.txt` at your installation (the checked-in defaults reference a local Windows Boost layout and may need adjustment on Linux). On Windows, `ws2_32` is linked as required by Boost.Asio (used by Boost.Process).

## Generated Project Layout

Running `prom new -pj <path>` produces:

```text
<path>/
├── CMakeLists.txt
├── configuration.json
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

The generated `CMakeLists.txt` targets C++23 by default (configurable), and on Windows can optionally embed `app.rc` as the application icon — pass `--rc` (or `rc`) to `prom new` to enable it. The note printed by `prom new` explains how the icon file itself must be provided. The generated `configuration.json` is pre-filled with the standard defaults, so all commands work right away.

## Project Configuration (`configuration.json`)

Each project carries a `configuration.json` in its root directory. It is created automatically by `prom new` and by `prom init`, and it defines the paths prom uses for the project:

```json
{
    "default_debug": "out/build/x64-debug",
    "default_pkg": "out/packages",
    "default_release": "out/build/x64-release",
    "exe": "prom"
}
```

- `exe` — default executable name; `run` builds and then executes `<build_dir>/<exe>`.
- `default_debug` — build directory used by `build` / `run` / `update` / `pack` in debug mode.
- `default_release` — build directory used by `build` / `run` / `update` / `pack` in release mode.
- `default_pkg` — directory where `pack` places the generated packages.

Paths are resolved relative to the project root. Edit the file to change the layout; the `pack`, `update`, `build`, and `run` commands all honor these values (unless a `-b <build_path>` overrides the build directory). If the file is missing or a key is empty, those commands report an error and suggest running `init -pj <project_root_path>`.

## Usage

```text
prom <command> [options]
prom help
```

Since 1.01, options may be given in **any order** — the parser no longer requires a fixed positional layout. The project path is supplied with `-pj <path>` (since 1.03 it may be omitted for `build` / `run` / `update` / `pack`, defaulting to the current working directory). Build modes are selected with `--debug` / `--release` (or their bare forms `debug` / `release`), and conflicting mode flags cannot be combined. Since 1.03, `build` / `run` / `update` / `pack` also accept `-b <build_path>` to point at a specific build directory (the one containing `CMakeCache.txt`).

### `new` — create a new project

```text
prom new -pj <project_path> [<build_mode>] [--rc]
```

- `<build_mode>`: `--debug` | `--release` | `--both` (default: `--debug`)
- `--rc` (or `rc`): enable the Windows resource file / application icon in the generated project
- Creates the project skeleton described above (including `configuration.json`) and runs an initial CMake configure for the selected build mode(s).

### `init` — create the project configuration

```text
prom init -pj <project_root_path>
```

- Generates a `configuration.json` in the project's root directory, pre-filled with the standard defaults (`exe` set to the project folder name, `default_debug` to `out/build/x64-debug`, `default_release` to `out/build/x64-release`, `default_pkg` to `out/packages`).
- Required for existing projects created with versions before 1.04, which do not yet have a `configuration.json`.

### `build` — build an existing project

```text
prom build [-pj <project_root_path>] [-b <build_path>] [<build_type>] [--verbose]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `--verbose` (or `verbose`): enable verbose build output
- `-b <build_path>` (since 1.03): build the given build directory (where `CMakeCache.txt` lives) instead of the default from `configuration.json`. Note: the build type recorded in `CMakeCache.txt` then takes precedence over `--debug` / `--release`.
- Build directories default to `default_debug` / `default_release` from `configuration.json`.
- Runs `cmake --build` against the selected build directory.

### `run` — build and run

```text
prom run [-pj <project_root_path>] [-b <build_path>] [<build_type>] [--verbose]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `-b <build_path>` (since 1.03): build into and execute from the given build directory; the build type recorded in `CMakeCache.txt` takes precedence over `--debug` / `--release` (a warning is printed).
- The executed executable name is taken from the `exe` key in `configuration.json` (e.g. `myapp.exe` on Windows). The `-bin <executable_name>` option overrides it when supplied.
- Builds the project for the selected build type, then automatically executes the binary from the corresponding build directory.

### `update` — reconfigure an existing project

```text
prom update [-pj <project_root_path>] [-b <build_path>] [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `-b <build_path>` (since 1.03): reconfigure the given build directory instead of the default from `configuration.json`.
- Build directories default to `default_debug` / `default_release` from `configuration.json`.
- Re-runs the CMake configure step (e.g. after modifying `CMakeLists.txt`).

### `pack` — package a built project

```text
prom pack [-pj <project_root_path>] [-b <build_path>] [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--release`)
- `-b <build_path>` (since 1.03): package using `<build_path>/CPackConfig.cmake` instead of the default build directory's `CPackConfig.cmake`.
- The build directory defaults to `default_debug` / `default_release` from `configuration.json`, and output is placed in the `default_pkg` directory.
- Runs `cpack`, placing output in `out/packages` by default.

### `show` — license information

```text
prom show w   # show warranty disclaimer
prom show c   # show redistribution conditions
```

### `--version` / `-v` / `--v`

Prints the current prom version (`1.04`).

### `help`

Prints a summary of all commands, including the new `init` syntax.

## Configuration

prom uses two configuration files:

- **`config.json`** — stored next to the prom executable. Holds the resolved paths of `cmake`, `ninja`, and `cpack`. On first run, prom attempts to auto-detect these tools on your system `PATH`; any tool it can't find is requested interactively. If `config.json` becomes invalid or a stored path no longer exists, prom automatically attempts to re-detect and repair it on the next run.
- **`configuration.json`** — stored in each project's root directory. Holds the project's default debug/release build directories, packages output directory, and default executable name (see [Project Configuration](#project-configuration-configurationjson)). Created by `prom new` and `prom init`, and required by `build` / `run` / `update` / `pack`.

## License

Copyright (C) 2026 Yvhang Cai (Jeffrey Miller)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
