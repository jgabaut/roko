# roko

  A VM written in C.

  Small personal project.

## Table of Contents <a name = "index"></a>

+ [Building](#building)
  + [Dependencies](#dependencies)
  + [Initialitising Submodules](#init_submodules)
  + [Autoreconf and ./configure](#autotools)
  + [Build with mingw32](#mingw32_build)
  + [Compiling](#compiling)
+ [Installation](#installation)
+ [Generating Documentation](#make_doc)
+ [Latest Release](#latest_release)


## Building <a name = "building"></a>

  The code should build for:

  - `x86_64-Linux`
  - `aarch64-Linux`(see [this section](#latest_release))
  - `aarch64-apple-darwin`
  - `Windows` (through `x86_64-w64-mingw32`)

  **DISCLAIMER:**  The `Windows` build is not tested.

### Dependencies <a name = "dependencies"></a>

  - `autoconf` and `automake` are needed to generate the Makefile used to compile the game.

### Initialising submodules <a name = "init_submodules"></a>

  You can initialise the submodules by running:

    `git submodule update --init`

### Autoreconf and ./configure <a name = "autotools"></a>

  You may need to run `automake --add-missing` to ensure your system provides the missing files needed by `autoreconf`.

  Run `autoreconf` to generate the `./configure` script. Run the script to generate the `Makefile` used for the project.

### Enable game debug  <a name = "debug_access"></a>

  After generating the `./configure` script, you may enable game debug mode by running:

    `./configure --enable-debug=yes`

  Which should prepare a `Makefile` with the correct macro definitions to turn on game debug at compile time. To reset the `Makefile` to release state, run:

    `./configure --enable-debug=no`

### Build with `x86_64-w64-mingw32` <a name = "mingw32_build"></a>

  After generating the `./configure` script, you may switch target host and use `mingw32` with:

    `./configure --host x86_64-w64-mingw32`

  Which should prepare a `Makefile` with the correct macro definitions. To reset the `Makefile` to release state, run:

    `./configure --host $(./config.guess)`

  You may need to correct the `configure.ac` flags if your installation does not have the expected layout, providing the correct paths for:.

  - `mingw32`'s `lib` and `include` dirs,
  - `mingw32`'s correct `gcc`.

### Compiling and running <a name = "compiling"></a>

  Once you have the `Makefile`, you can compile the project by running, in its directory:

    `make`

  The build chain expects some files to be generated from already given ones.

  If you want to build without using `make`, you can see in the `Makefile.am` how to generate `./src/anvil__roko.c` and `./src/anvil__roko.h`.

## Generating documentation <a name = "make_doc"></a>

  You can use `doxygen` to generate docs in both HTML and latex.

  To generate the documentation run, *from the repo dir*:

  ```
  doxygen docs/roko.doxyfile
  ```

  To have a pdf version, you can install a tex compiler and, inside `doxygen/latex`, run:
  ```
  make
  ```

  Running `make doc` should give you `./docs/docs.pdf`, just the pdf output from latex, and delete the html index by itself.

## Latest Release <a name = "latest_release"></a>

  I try to upload precompiled binaries for:

  - `x86_64-Linux`
  - `aarch64-Linux` (from [Termux](https://f-droid.org/packages/com.termux/) on Android).
  - `x86_64-w64-mingw32` (*NOT TESTED*).

  Some releases may offer a precompiled binary for:

  - `darwin-arm64`

  📦 v0.1.1 29/10/2023
  https://github.com/jgabaut/roko/releases
