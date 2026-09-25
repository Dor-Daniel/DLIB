# dbld

`dbld.h` is a header-only, **incremental C build system for Windows**. It scans a source tree, figures out which files changed (including through header dependencies), compiles only what's needed in parallel, links the result, and can remember your settings between runs so a bare `.\dbuild.exe` rebuilds your project with no flags at all.

It exposes a single function:

```c
int dbuild(const int argc, char* argv[]);
```

Depending on how you compile the header, you either get a ready-to-run executable automatically, or you call this function yourself from your own `main`. See [Getting the executable](#getting-the-executable) for both paths.

---

## Table of contents

1. [What it does and how it operates](#what-it-does-and-how-it-operates)
2. [Feature set right now](#feature-set-right-now)
3. [Requirements](#requirements)
4. [Getting the executable](#getting-the-executable)
5. [Command-line reference](#command-line-reference)
6. [How a build runs, step by step](#how-a-build-runs-step-by-step)
7. [On-disk layout: config and caches](#on-disk-layout-config-and-caches)
8. [Project templates](#project-templates)
9. [Embedding dbld.h in a larger program](#embedding-dbldh-in-a-larger-program)
10. [Compile-time configuration](#compile-time-configuration)
11. [Known issues](#known-issues)

---

## What it does and how it operates

At its core, `dbld` is a wrapper around `gcc` that adds three things gcc doesn't do on its own: incremental rebuilds, parallel compilation, and a persisted configuration.

**Incremental rebuilds.** Every file under your source directory (`.c`, `.h`, anything else) is recorded in a JSON cache (`build/<target>/files.json`) with its last-write time and, for source files, the list of headers it depends on (worked out by running `gcc -MM` on it once). On a later run, `dbld` compares the recorded metadata against the files that exist now. A `.c` file is scheduled for recompilation if:
- it is new or its own last-write time changed, or
- any header it depends on changed, or
- it was named with `-ff` / `--force-file`.

**Parallel compilation.** Every file that needs compiling is launched as its own `gcc` child process (via `CreateProcessA`) essentially all at once, and `dbld` waits for all of them (`WaitForMultipleObjects`) and checks each one's exit code before deciding whether to link.

**One link step.** Once compilation finishes successfully, `dbld` links every `.o` file in `build/<target>/object_files/` into the final debug executable, release executable, or shared library with a single `gcc` call.

**Persisted configuration.** Everything you'd otherwise type on the command line — source/build directories, output file names, extra compiler/linker flags, include paths, pre/post-build commands, which template to use — can be saved with `-s` into a per-target `config.json`, and reloaded on later runs with `-uc`, or automatically when you run `dbld` with no arguments at all in a project you've already configured.

**Refreshing the cache after a build.** After a build finishes, `dbld` relaunches itself with a hidden pseudo-argument (`###object-files###`) to rescan the source tree and rewrite `files.json` for next time. Don't pass this flag manually — it's used internally like forking without a copy of the PCB, and passing it yourself can crash the process. This is a fire-and-forget child process: `dbld` does not wait for it or check that it succeeded before your own build run exits.

**No args in a non-empty folder** always operates against the **debug** target's saved configuration specifically (not whichever target you last used), because the target defaults to `DBUILD_TARGET_DEBUG` before any saved config for a *different* target could apply. See [How a build runs](#how-a-build-runs-step-by-step) for the exact logic.

---

## Feature set right now

Everything below is implemented and reachable through the CLI:

- Three build targets: **debug** (`-g`), **release** (`-Ofast`), **shared library** (`-shared`, produces a `.dll`), each with its own output folder, cache and saved config.
- Incremental recompilation based on file modification time **and** header dependencies (via `gcc -MM`).
- Parallel compilation of all files that need it, one `gcc` process per file, with a real per-file success check afterward — see [How a build runs](#how-a-build-runs-step-by-step).
- Forcing specific files to recompile regardless of cache state (`-ff`).
- Extra compiler flags (`-cf`) and linker flags (`-lf`), each with its own independent limit (see [Compile-time configuration](#compile-time-configuration)).
- Include directories (`-I`), link directories (`-L`), and libraries to link (`-l`), each with its own independent limit too.
- Custom source directory (`-src`) and build output directory (`-bld`).
- Custom output file names per target (`-od`, `-or`, `-os`), validated to end in `.exe` (debug/release) or `.dll` (shared).
- Pre-build and post-build shell commands (`-pr`, `-po`). The pre-build command now runs to completion before compilation starts; the post-build command is fired off (without waiting) only if linking succeeded.
- Cleaning a target's build cache (`-c`).
- Saving (`-s`) and loading (`-uc`) the full configuration above, per target, as JSON.
- Verbose/info printing during a run (`-i`): confirms a config finished loading, names each file as it's sent to the compiler, and reports the compile/link stages ending successfully.
- Generating a starter project: a `.vscode/` setup (`-bt-vscode`) or a `.clangd` config (`-bt-clangd`), plus a `src/main.c` "Hello World" for the vscode template. This also happens automatically the first time you run `dbld` in an empty folder.
- Help text (`-h`).
- Building the whole thing, including a working `main`, from a **single compiler invocation** — no wrapper source file required. See [Getting the executable](#getting-the-executable).

Not implemented yet: multithreading and a bound on how many compiler processes run at once — see [Known issues](#known-issues). This is also a Windows-only tool — see [Requirements](#requirements).

---

## Requirements

- **Windows.** The implementation is built entirely on the Win32 API: `CreateProcessA`, `FindFirstFileA`/`FindNextFileA`, `SHFileOperationA`, `GetCurrentDirectoryA`, `SYSTEMTIME`, and so on. There is no fallback for other platforms.
- **GCC** (a MinGW-w64 toolchain, in practice) reachable as plain `gcc` on `PATH`. It's used both to compile/link and, via `gcc -MM`, to compute header dependencies.
- **`popen`/`where`** working from your shell environment (used for `gcc -MM` output and, only for the VS Code template, to locate `gcc` and `gdb`).
- **`gdb`** on `PATH` only if you want the generated `.vscode/launch.json` to have a working debugger path; it's not required to build.
- The three sibling headers listed in [Embedding dbld.h in a larger program](#embedding-dbldh-in-a-larger-program) — needed even for the single-command path below, since `dbld.h` `#include`s them directly.

---

## Getting the executable

`dbld.h` has no unconditional `main` — but it can generate one for you.

### The simple way: one compiler invocation

Defining **`DBUILD_STANDALONE`** does four things at once: it turns on the implementation for `dbld.h` itself, and for its three dependencies (`dstring.h`, `dhash_table.h`, `json_parser.h`), and it supplies a `main` that just calls `dbuild(argc, argv)`. So if you already have the full header bundle laid out the way `dbld.h` expects (see the sibling-header layout in [Embedding dbld.h](#embedding-dbldh-in-a-larger-program) — `DBUILD_STANDALONE` doesn't remove that requirement, it just automates turning all four headers' implementations on), you can go straight from the header to a working `dbuild.exe`:

```bat
gcc -x c -DDBUILD_STANDALONE dbld.h -o dbuild.exe -lshell32
```

`-x c` tells `gcc` to compile the `.h` file as C source directly (compilers otherwise treat `.h` as a header to be included, not compiled). `-lshell32` is needed for `SHFileOperationA`, used by `-c`/`--clean`.

### The embedding way: your own main

If you'd rather call `dbuild()` from inside a larger program of your own — or just don't want the automatic `main` — define `DBUILD_IMPLEMENTATION` instead, and write the two lines yourself:

```c
// dbuild.c
#define DBUILD_IMPLEMENTATION
#include "path/to/dbld.h"

int main(int argc, char* argv[])
{
    return dbuild(argc, argv);
}
```

```bat
gcc dbuild.c -o dbuild.exe -lshell32
```

This is also the path to use if you're pulling `dbld.h` into a project that already defines its own `*_IMPLEMENTATION` for `dstring.h`/`dhash_table.h`/`json_parser.h` elsewhere, since `DBUILD_STANDALONE` turns all three on unconditionally alongside `dbld.h`'s own.

### Either way

From here on, everything in this document — every flag, every behavior — is about using the resulting `dbuild.exe`, exactly the way you'd use a prebuilt tool. Drop it into a project folder (or somewhere on `PATH`) and run it.

---

## Command-line reference

```
dbuild.exe [Options]
```

Running it **with no arguments** does one of three things, depending on the current directory:

| Current directory | Behavior |
|---|---|
| Empty (ignoring `dbuild.exe` itself) | Generates a new default project template (see [Project templates](#project-templates)). |
| Not empty, and the **debug** target has a saved config (`build/debug/config.json` exists) | Loads that config and builds the debug target. |
| Not empty, no saved debug config | Prints the help message. |

### Options

| Short | Long | Takes a value | What it does |
|---|---|---|---|
| `-bd` | `--build-debug` | – | Select the debug target for this run. |
| `-br` | `--build-release` | – | Select the release target for this run. |
| `-bs` | `--build-shared` | – | Select the shared-library target for this run. |
| `-od` | `--output-debug` | a file name ending in `.exe` | Set the debug target's output executable name. |
| `-or` | `--output-release` | a file name ending in `.exe` | Set the release target's output executable name. |
| `-os` | `--output-shared` | a file name ending in `.dll` | Set the shared target's output library name. |
| `-c` | `--clean` | – | Delete the selected target's `object_files/` folder and `files.json` cache before building. |
| `-s` | `--save-config` | – | Save the current settings as the selected target's config. |
| `-uc` | `--use-config` | – | Load the selected target's saved config before building. |
| `-h` | `--help` | – | Print the help message and exit immediately. |
| `-cf` | `--compiler-flag` | a flag | Add one flag to every compile step. Repeatable. |
| `-lf` | `--linker-flag` | a flag | Add one flag to the link step. Repeatable. |
| `-i` | `--info` | – | Print extra info while scanning, compiling and linking. |
| `-bt-vscode` | `--build-template-vscode` | – | Generate the VS Code starter template. |
| `-bt-clangd` | `--build-template-clangd` | – | Generate the clangd starter template. |
| `-src` | `--source-directory` | a directory | Set the source directory (default: `src`). |
| `-bld` | `--build-directory` | a directory | Set the build output directory (default: `build`). |
| `-ff` | `--force-file` | a file path | Force this file to recompile even if the cache says it's unchanged. Repeatable, up to a compile-time limit. |
| `-I` | `--include-directory` | a directory | Add an include directory for compilation. Repeatable. |
| `-L` | `--link-directory` | a directory | Add a library search directory for linking. Repeatable. |
| `-l` | `--link-library` | a library name | Add a library to link against (passed to gcc as `-lname`). Repeatable. |
| `-pr` | `--pre-build` | a command string | Run this shell command before compiling, and wait for it to finish. |
| `-po` | `--post-build` | a command string | Run this shell command after a successful link, without waiting for it. Skipped if linking failed. |

Options that take a value must be followed immediately by that value as the next argument. Passing one of these flags as the last argument prints an error and exits.

Anything that doesn't start with `-`, or an unrecognized flag, produces a warning and (the first time only) the help text, but does not stop the run.

### Examples

```bat
:: Build the release target using the saved config.
dbuild.exe -br

:: Same, plus an extra compiler flag and verbose scanning.
dbuild.exe -br -cf -Wall -i

:: Build debug, wiping the debug build cache first, with info printing.
dbuild.exe -bd -c -i

:: Build the shared target, adding an include path.
dbuild.exe -bs -I C:/libs/my_lib/include

:: Build the shared target, add a link directory, and save these settings
:: as the new saved config for the shared target.
dbuild.exe -s -bs -L C:/libs/my_lib/lib
```

---

## How a build runs, step by step

For a normal invocation with arguments (not the special `###object-files###` re-entry, and not the "no arguments" shortcuts):

1. **Parse arguments** (`parse_args`). This updates an in-memory context (`CTX`) with the target, flags, directories, and so on. `-h` exits immediately here.
2. **Apply saved config, if asked.**
   - If `-s` was passed: create the build directory tree, then write the current `CTX` out as `build/<target>/config.json`.
   - Else if `-uc` was passed: load `build/<target>/config.json` into `CTX` (this **overwrites** whatever you set with other flags on this same run for the fields present in the file), then create the build directory tree. If no config was ever saved for that target, this is a hard error. With `-i` set, a successful load prints a confirmation line.
3. **Scan the source directory** (`scan_source_directory`), recursively recording every file's path, type (`.c`/`.h`/other) and last-write time into an in-memory hash table.
4. **Run the pre-build command**, if one is set (`-pr` or loaded from config), and wait for it to finish before moving on.
5. **Compare against the cache**: `load_existings_object_files` reads the previous `files.json` (if any), and for every file whose last-write time differs from what's recorded, marks it `is_changed`. `filter_scanned_files_to_compile` then marks a source file `should_compile` if it's forced (`-ff`), or if it or any of its recorded dependencies changed.
6. **Compile**: every file marked `should_compile` is compiled in its own `gcc` child process, all launched together (with `-i`, each is announced as it's sent off). `dbld` waits for every process to finish, then checks each one's exit code individually, so a file that failed to compile is caught here rather than surfacing later as a confusing link error.
7. **Link**: a single `gcc` call links every object file currently in `build/<target>/object_files/` into the target's output file; its exit code is checked.
8. **Run the post-build command**, if one is set, but only if linking succeeded. This one is fired off without waiting for it to finish.
9. **Refresh the cache**: `dbld` relaunches itself with the hidden `###object-files###` argument to rescan the source tree (recomputing each file's dependencies via `gcc -MM`) and rewrite `files.json`, without waiting for that relaunch to finish.

For a **no-argument run** in a non-empty, already-configured folder, steps 2–9 run the same way, but always against the **debug** target's saved config specifically (step 1 and the `-uc`/`-s` branch are skipped; `should_use_config` is forced on for debug).

---

## On-disk layout: config and caches

```
your-project/
├── src/                         (default; -src to change)
│   └── main.c
└── build/                       (default; -bld to change)
    ├── debug/
    │   ├── config.json          (written by -s, read by -uc, while target = debug)
    │   ├── files.json           (per-file metadata + dependency cache)
    │   └── object_files/        (compiled .o files + linked main.exe)
    ├── release/
    │   ├── config.json
    │   ├── files.json
    │   └── object_files/        (main.exe by default — separate folder from debug's)
    └── shared/
        ├── config.json
        ├── files.json
        └── object_files/        (main.dll by default)
```

All three target folders (and their `object_files/` subfolders) are created up front by `create_build_directory`, regardless of which single target you're building this run.

`-c` / `--clean` deletes only the **selected** target's `object_files/` folder and `files.json` — the config, and the other two targets, are untouched.

Config and cache files are **per target**: saving a config while building release (`-s -br`) does not affect the debug or shared configs, and each target tracks its own compiled-file cache independently.

---

## Project templates

Templates are generated by `-bt-vscode` / `-bt-clangd`, or automatically on the first run in an empty directory (using whichever template is the compile-time default — see [Compile-time configuration](#compile-time-configuration)).

**`-bt-vscode`** creates:
- `.vscode/launch.json` — two debug configurations ("Debug" and "Clean-Cache & Debug"), pointing at the debug target's output and using `gdb` (located via `where gdb` at template-generation time).
- `.vscode/tasks.json` — three build tasks: "Build & Run (Release)", "Build (Debug)", and "Build & Clean (Debug)", each invoking your compiled `dbuild.exe` with the matching flags.
- `.vscode/settings.json` — Code Runner settings pointing at the debug executable, file associations for `.c`/`.h`, a save-triggered build task, and some editor preferences.
- `.vscode/c_cpp_properties.json` — include path pointing at your source directory, using `gcc` located via `where gcc`.
- `.vscode/extensions.json` — a handful of recommended extensions (C/C++ tools, Code Runner, Error Lens, GitLens, etc.).
- `src/main.c` — a minimal `"Hello World!"` program, only if it doesn't already exist as part of this step.

**`-bt-clangd`** creates a single `.clangd` file with your source directory added as an include path.

---

## Embedding dbld.h in a larger program

### Dependencies

`dbld.h`'s implementation needs three sibling headers to exist relative to it, whether you build it standalone or embed it:

```
your-project/
└── include/
    ├── ddefines.h                       (u8/u32/u64/bool etc., same as dstring's)
    ├── data_structures/
    │   ├── dstring.h
    │   └── dhash_table.h
    └── parsers/
        └── json_parser.h
```

The exact relative paths the header uses are `"../data_structures/dstring.h"`, `"../data_structures/dhash_table.h"`, and `"../parsers/json_parser.h"` — so wherever you put `dbld.h`, those two folders (`data_structures/` and `parsers/`) need to be *siblings* of the folder `dbld.h` lives in. `dstring.h` is documented separately; see its own README if you have one.

You'll also need `windows.h` and `shellapi.h`, which come with the Windows SDK / MinGW.

### Enabling the implementation

Two macros matter here, and they do different things:

- **`DBUILD_STANDALONE`** — turns on `DBUILD_IMPLEMENTATION` *and* `DSTRING_IMPLEMENTATION`, `JSON_PARSER_IMPLEMENTATION`, `DHASH_TABLE_IMPLEMENTATION` together, and supplies a `main`. Use this for the single-invocation build described in [Getting the executable](#getting-the-executable). Because it provides its own `main`, don't combine it with a `main` of your own in the same translation unit.
- **`DBUILD_IMPLEMENTATION`** — turns on only `dbld.h`'s own implementation. You provide `main`, and you're responsible for making sure `dstring.h`, `dhash_table.h` and `json_parser.h` have their implementations enabled somewhere in your build (each has its own `*_IMPLEMENTATION` guard, the same convention `dbld.h` follows).

```c
// dbuild.c
#define DBUILD_IMPLEMENTATION
#include "dbld.h"

int main(int argc, char* argv[])
{
    return dbuild(argc, argv);
}
```

Every other file that needs the `dbuild()` declaration just includes `dbld.h` normally, without either define.

### Compiling

```bat
gcc dbuild.c -o dbuild.exe -lshell32
```

If you see undefined references, check that exactly one translation unit defines `DBUILD_IMPLEMENTATION` (or `DBUILD_STANDALONE`), and that the sibling headers above are reachable at their expected relative paths with their own implementations enabled.

---

## Compile-time configuration

All of these can be overridden by `#define`-ing them before you `#include "dbld.h"` in your implementation file (or via `-D` on the compiler command line — including alongside `-DDBUILD_STANDALONE`). **`--help` only documents the first 10 of these** — the other six exist but currently aren't mentioned by `--help` at all.

| # | Macro | Default | Purpose |
|---|---|---|---|
| 1 | `DBUILD_MEMORY_SIZE` | `MB(1)` (1,048,576 bytes) | Backing store for a simple bump allocator used for *all* of `dbld`'s own allocations. If you hit an `Out-Of-Memory` error, raise this. |
| 2 | `DBUILD_MAX_COMPILER_EXTRA_ARGS` | 32 | Max number of `-cf` flags. |
| 3 | `DBUILD_MAX_FILES_COUNT` | `KB(1)` (1024) | Max total files (source, header, or otherwise) trackable under the source directory. Also sizes the per-build arrays used to launch and wait on compiler processes. |
| 4 | `DBUILD_TEMPLATE_DEFAULT` | `DBUILD_TEMPLATE_VSCODE` | Which template is generated automatically when run with no args in an empty directory. |
| 5 | `DBUILD_BUILD_TARGET_DEFAULT` | `DBUILD_TARGET_DEBUG` | The target used when no `-bd`/`-br`/`-bs` is given, and always the target used for a no-argument run. |
| 6 | `DBUILD_BUILD_DIRECTORY_DEFAULT` | `"build"` | Default build output directory. |
| 7 | `DBUILD_SOURCE_DIRECTORY_DEFAULT` | `"src"` | Default source directory. |
| 8 | `DBUILD_DEBUG_COMPILATION_FLAG` | `"-g"` *(not in `--help`)* | Flag passed to gcc when compiling/linking the debug target. |
| 9 | `DBUILD_RELEASE_COMPILATION_FLAG` | `"-Ofast"` *(not in `--help`)* | Flag passed to gcc when compiling/linking the release target. |
| 10 | `DBUILD_SHARED_COMPILATION_FLAG` | `"-shared"` *(not in `--help`)* | Flag passed to gcc when compiling/linking the shared-library target. |
| 11 | `DBUILD_MAX_FORCED_FILES` | 32 | Max number of `-ff` entries in a single run. |
| 12 | `DBUILD_MAX_INCLUDE_DIRECTORIES` | 32 | Max entries for `-I`. |
| 13 | `DBUILD_MAX_LINKER_EXTRA_ARGS` | 32 *(not in `--help`)* | Max number of `-lf` flags. Independent of `DBUILD_MAX_COMPILER_EXTRA_ARGS`. |
| 14 | `DBUILD_MAX_LIBS_INCLUDE_DIRECTORIES` | 32 *(not in `--help`)* | Max entries for `-L`. Independent of `DBUILD_MAX_INCLUDE_DIRECTORIES`. |
| 15 | `DBUILD_MAX_LIBS` | 32 *(not in `--help`)* | Max entries for `-l`. Independent of the other two limits above. |
| 16 | `DBUILD_MAX_DEPENDENCIES_COUNT` | 64 | Max headers tracked as dependencies of a single source file. |

`-cf`/`-lf` and `-I`/`-L`/`-l` each have their own independent capacity — raising one limit macro doesn't affect an unrelated option.

---

## Known issues

Things I still need to fix, kept here as a single, clean list so each one can just be deleted once it's done.

- **No cap on how many compiler processes run at once**, and no reuse of them as a pool. `build()` still launches one `gcc` process per file that needs compiling, all at once, bounded only by `DBUILD_MAX_FILES_COUNT`.
- **Still depends on three sibling headers** (`dstring.h`, `dhash_table.h`, `json_parser.h`) instead of being a true single header. Fine for now, but it's on the list to fold in directly.
