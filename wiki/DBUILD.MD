# dbuild.h

**You can Find The source Code [here](../src/utils/dbuild.h)**

 
A small, single-header C build tool. It compiles the files in your source directory, links them into a debug, release, or shared-library target, and can save your settings so the next build is a single short command.
 
This README has two parts:
 
- [**Using the executable**](#part-1-using-the-executable) - you already have `dbuilder.exe` and just want to build your project.
- [**Building dbuild from source**](#part-2-building-dbuild-from-source) - you want to compile the builder yourself, or tune its built-in limits.
---
 
## Table of contents
 
- [Part 1: Using the executable](#part-1-using-the-executable)
  - [Quick start](#quick-start)
  - [Running with no arguments](#running-with-no-arguments)
  - [Project layout](#project-layout)
  - [Options reference](#options-reference)
  - [Examples](#examples)
  - [Saving and reusing configurations](#saving-and-reusing-configurations)
  - [Editor templates](#editor-templates)
- [Part 2: Building dbuild from source](#part-2-building-dbuild-from-source)
  - [Steps](#steps)
  - [Compile-time configuration](#compile-time-configuration)
  - [Troubleshooting](#troubleshooting)
---
 
# Part 1: Using the executable
 
## Quick start
 
1. Put `dbuilder.exe` somewhere convenient (in your project folder, or on your `PATH`).
2. Open a terminal in your project folder.
3. Run it:
```
   .\dbuilder.exe
```
 
What happens next depends on the state of the folder - see [Running with no arguments](#running-with-no-arguments).
 
To see the full built-in help at any time:
 
```
.\dbuilder.exe -h
```
 
## Running with no arguments
 
`.\dbuilder.exe` with no arguments picks its behavior from the current directory:
 
| Current directory state | What dbuild does |
|---|---|
| **Empty** | Generates a new default project template. |
| **Not empty**, and you have saved a configuration at least once | Runs the saved configuration. |
| **Not empty**, and no configuration was ever saved | Fails & Prints the help message. |
 
## Project layout
 
By default dbuild expects:
 
| Directory | Purpose | Default | Override with |
|---|---|---|---|
| Source directory | Your source files. **Must exist** - dbuild reports an error if it is missing. | `src` | `-src` / `--source-directory` |
| Build directory | Compiler and linker output. Created automatically if missing. | `build` | `-bld` / `--build-directory` |
 
Cleaning a target with `-c` removes that target's folder inside the build directory (for example, `build/debug`).
 
## Options reference
 
### Choosing a target
 
| Option | Long form | Description |
|---|---|---|
| `-bd` | `--build-debug` | Add the define that marks the build target as **debug**. |
| `-br` | `--build-release` | Add the define that marks the build target as **release**. |
| `-bs` | `--build-shared` | Add the define that marks the build target as a **shared library**. |
 
### Naming the output

**NOTE:** *Should be followd with an extra argument with the actual name*
 
| Option | Long form | Description |
|---|---|---|
| `-od` | `--output-debug` | Set the output file name for the debug target. |
| `-or` | `--output-release` | Set the output file name for the release target. |
| `-os` | `--output-shared` | Set the output file name for the shared-library target. |
 
### Compiler and linker settings
 
Options marked **takes a value** must be followed *immediately* by their value.
 
| Option | Long form | Takes a value | Description |
|---|---|---|---|
| `-cf` | `--compiler-flag` | a flag | Add a flag to the compiler command. |
| `-lf` | `--linker-flag` | a flag | Add a flag to the linker command. |
| `-I` | `--include-directory` | a directory path | Add an include directory to the compilation step. |
| `-L` | `--link-directory` | a directory path | Add a library directory to the linker step. |
| `-l` | `--link-library` | a library name | Add a library to link against. |
 
### Build behavior
 
| Option | Long form | Takes a value | Description |
|---|---|---|---|
| `-c` | `--clean` | - | Clean all defined targets. |
| `-t` | `--threads` | a number | Maximum number of threads to use. The actual number used is `min(available, required)`. |
| `-ff` | `--force-file` | a file name | Force this file to be recompiled even if it has no changes. |
| `-i` | `--info` | - | Print extra information while building. |
| `-src` | `--source-directory` | a directory path | Set the source directory (default: `src`). |
| `-bld` | `--build-directory` | a directory path | Set the build directory (default: `build`). |
 
### Build tasks
 
| Option | Long form | Takes a value | Description |
|---|---|---|---|
| `-pr` | `--pre-build` | a command string | Run a task **before compiling** |
| `-po` | `--post-build` | a command string | Run a task **after linking**. It does **not** run if linking failed. |
 
Wrap command strings in quotes if they contain spaces.
 
### Configuration and templates
 
| Option | Long form | Description |
|---|---|---|
| `-s` | `--save-config` | Save the current configuration. |
| `-uc` | `--use-config` | Load the saved configuration and use it in this build. |
| `-bt-vscode` | `--build-template-vscode` | Generate a quick-start template for VS Code. |
| `-bt-clangd` | `--build-template-clangd` | Generate a quick-start template for clangd. |
 
### Help
 
| Option | Long form | Description |
|---|---|---|
| `-h` | `--help` | Print the help message and exit. |
 
## Examples
 
Build the **release** target with your saved configuration:
 
```
.\dbuilder.exe -br -uc
```
 
Build release, add `-Wall` to the compiler flags, and print progress info:
 
```
.\dbuilder.exe -br -cf -Wall -i
```
 
Build the **debug** target, clean `build/debug` first, and print progress info:
 
```
.\dbuilder.exe -bd -c -i
```
 
Build the **shared** target and add an extra include path for compilation:
 
```
.\dbuilder.exe -bs -I C:/libs/my_lib/include
```
 
Build the **shared** target, add an extra library path for linking, and **save** these settings so they become your new configuration:
 
```
.\dbuilder.exe -s -bs -L C:/libs/my_lib/lib
```
 
Link against a library and run a post-build step:
 
```
.\dbuilder.exe -br -l m -po "echo build finished"
```
 
Force a single file to recompile and use up to 8 threads:
 
```
.\dbuilder.exe -bd -ff main.c -t 8
```
 
## Saving and reusing configurations
 
1. Run a build with the settings you want, adding `-s`:
```
   .\dbuilder.exe -s -bd -I include -l m
```
 
2. From then on, `.\dbuilder.exe` (with no arguments) in that folder re-runs your saved configuration.
3. Any options you pass on a later run are applied to *that* build. Add `-s` again if you want them to replace the saved configuration.
## Editor templates
 
To generate a quick-start setup for your editor:
 
```
.\dbuilder.exe -bt-vscode
.\dbuilder.exe -bt-clangd
```
 
Running `.\dbuilder.exe` in an **empty** folder generates the template that the builder was compiled to use by default (VS Code unless it was rebuilt with a different `DBUILD_TEMPLATE_DEFAULT`).
 
---
 
# Part 2: Building dbuild from source
 
dbuild is a single-header library. You build the builder by creating a small source file that includes the header with the implementation enabled.
 
## Steps
 
### 1. Get the files
 
You need `dbuild.h` in your working directory.
 
### 2. Create `dbuild.c`
 
```c
#define DBUILD_IMPLEMENTATION
#include "dbuild.h"
```
 
`DBUILD_IMPLEMENTATION` **must** be defined before the header is included, exactly once, in exactly one source file.
 
### 3. Compile it
 
Use any C compiler. For example:
 
```
gcc dbuild.c -o dbuilder.exe
```
 
```
clang dbuild.c -o dbuilder.exe
```
 
```
cl dbuild.c /Fe:dbuilder.exe
```
 
### 4. (Optional) Override defaults
 
Any of the [compile-time settings](#compile-time-configuration) can be overridden by defining them before the header is included:
 
```c
#define DBUILD_MEMORY_SIZE          (4 * 1024 * 1024)
#define DBUILD_MAX_FILES_COUNT      4096
#define DBUILD_BUILD_TARGET_DEFAULT DBUILD_TARGET_RELEASE
 
#define DBUILD_IMPLEMENTATION
#include "dbuild.h"
```
 
Or pass them on the compiler command line:
 
```
gcc -DDBUILD_MEMORY_SIZE=4194304 -DDBUILD_MAX_FILES_COUNT=4096 dbuild.c -o dbuilder.exe
```
 
> **Note:** for `DBUILD_TEMPLATE_DEFAULT`, `DBUILD_BUILD_TARGET_DEFAULT`, `DBUILD_BUILD_DIRECTORY_DEFAULT`, and `DBUILD_SOURCE_DIRECTORY_DEFAULT`, check `dbuild.h` for the exact value format expected (an identifier, a string, or an enum name).
 
### 5. Use it
 
Continue with [Part 1](#part-1-using-the-executable).
 
## Compile-time configuration
 
| # | Macro | Default | Purpose |
|---|---|---|---|
| 1 | `DBUILD_MEMORY_SIZE` | 1 MB | Memory used for **all** allocations in the program. |
| 2 | `DBUILD_MAX_COMPILER_EXTRA_ARGS` | 32 | Maximum number of extra compiler arguments added with `-cf`. |
| 3 | `DBUILD_MAX_FILES_COUNT` | 1024 | Maximum number of source files in the source directory. |
| 4 | `DBUILD_TEMPLATE_DEFAULT` | vscode | Template generated when the builder runs in an empty folder with no arguments. |
| 5 | `DBUILD_BUILD_TARGET_DEFAULT` | debug | Target built when the builder runs in a non-empty folder with no arguments. |
| 6 | `DBUILD_BUILD_DIRECTORY_DEFAULT` | `build` | Default build directory. Created if it does not exist. |
| 7 | `DBUILD_SOURCE_DIRECTORY_DEFAULT` | `src` | Default source directory. If it does not exist, the builder reports an error. |
| 8 | `DBUILD_MAX_FORCED_FILES` | 32 | Maximum number of forced compilations requested with `-ff`. |
| 9 | `DBUILD_MAX_INCLUDE_DIRECTORIES` | 32 | Maximum number of include directories (`-I`). |
| 10 | `DBUILD_MAX_DEPENDENCIES_COUNT` | 64 | Maximum number of includes tracked per source file (including recursive includes). |
 
## Troubleshooting
 
| Symptom | Fix |
|---|---|
| Builder prints **`Out-Of-Memory`** | Increase `DBUILD_MEMORY_SIZE` and rebuild the builder. |
| Linker errors about undefined dbuild functions | Make sure `DBUILD_IMPLEMENTATION` is defined before including `dbuild.h`, in exactly one `.c` file. |
| Error about a missing source directory | Create the `src` folder, or point to yours with `-src <dir>`. |
| Too many source files, include directories, forced files, or extra compiler args | Raise the matching `DBUILD_MAX_*` macro and rebuild the builder. |
| Running with no arguments just prints help | The folder is non-empty and no config has been saved yet. Run once with `-s` plus your options to save one. |
| A post-build task didn't run | Post-build tasks are skipped when linking fails. Fix the link error first. |
 