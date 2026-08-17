# prom — Project Manager

**prom** is a lightweight CLI tool for scaffolding, configuring, building, running, updating, and packaging C++ projects that use CMake, Ninja, and CPack. It generates a ready-to-build project skeleton (with an optional Windows resource/icon file) and wraps the common CMake/Ninja/CPack workflow behind a handful of simple subcommands.

Current version: **1.03** (`PROJECT_MANAGER_VERSION`)

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
- **New `run` command** — `prom run` builds the project and then **automatically executes the produced binary** after the build finishes. Currently the executable name must still be specified manually via `-bin <executable_name>`; automatic detection of the built binary is planned for a future release.

## Features

- **Project scaffolding** (`new`) — creates a standard directory layout, a generated `CMakeLists.txt`, a starter `src/main.cpp`, and a Windows `app.rc` resource file (icon inclusion commented out by default, enable with `--rc`), then runs the initial CMake configure step.
- **Build** (`build`) — invokes `cmake --build` against the debug or release build directory (or a custom `-b` build directory), with optional verbose output.
- **Run** (`run`) — builds the project and then automatically executes the resulting binary.
- **Reconfigure** (`update`) — re-runs CMake configuration (`cmake -S ... -B ...`) for the debug or release build directory (or a custom `-b` build directory), useful after editing `CMakeLists.txt`.
- **Packaging** (`pack`) — runs `cpack` against the generated `CPackConfig.cmake` for the chosen build type (or a custom `-b` build directory).
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
- Creates the project skeleton described above and runs an initial CMake configure for the selected build mode(s).

### `build` — build an existing project

```text
prom build [-pj <project_root_path>] [-b <build_path>] [<build_type>] [--verbose]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `--verbose` (or `verbose`): enable verbose build output
- `-b <build_path>` (since 1.03): build the given build directory (where `CMakeCache.txt` lives) instead of the default `out/build/x64-debug` / `out/build/x64-release`. Note: the build type recorded in `CMakeCache.txt` then takes precedence over `--debug` / `--release`.
- Runs `cmake --build` against the selected build directory.

### `run` — build and run

```text
prom run [-pj <project_root_path>] [-b <build_path>] [<build_type>] [--verbose] -bin <executable_name>
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `-bin <executable_name>`: the name of the produced executable (e.g. `myapp.exe` on Windows) — **currently required to be specified manually**
- `-b <build_path>` (since 1.03): build into and execute from the given build directory; the build type recorded in `CMakeCache.txt` takes precedence over `--debug` / `--release` (a warning is printed).
- Builds the project for the selected build type, then automatically executes the binary from the corresponding build directory.

### `update` — reconfigure an existing project

```text
prom update [-pj <project_root_path>] [-b <build_path>] [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--debug`)
- `-b <build_path>` (since 1.03): reconfigure the given build directory instead of the default `out/build/x64-debug` / `out/build/x64-release`.
- Re-runs the CMake configure step (e.g. after modifying `CMakeLists.txt`).

### `pack` — package a built project

```text
prom pack [-pj <project_root_path>] [-b <build_path>] [<build_type>]
```

- `<build_type>`: `--debug` | `--release` (default: `--release`)
- `-b <build_path>` (since 1.03): package using `<build_path>/CPackConfig.cmake` instead of the default build directory's `CPackConfig.cmake`.
- Runs `cpack`, placing output in `out/packages`.

### `show` — license information

```text
prom show w   # show warranty disclaimer
prom show c   # show redistribution conditions
```

### `--version` / `-v` / `--v`

Prints the current prom version (`1.03`).

### `help`

Prints a summary of all commands, including the new `run` syntax.

## Configuration

On first run, prom looks for `config.json` next to its executable. If missing, it attempts to auto-detect `cmake`, `ninja`, and `cpack` on your system `PATH`; any tool it can't find is requested interactively. The resolved paths are then saved to `config.json` for future runs. If `config.json` becomes invalid or a stored path no longer exists, prom automatically attempts to re-detect and repair it on the next run.

## License

Copyright (C) 2026 Yvhang Cai (Jeffrey Miller)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
