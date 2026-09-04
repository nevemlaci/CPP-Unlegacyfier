# C++ Unlegacyfier and Refactor tool

This tool uses Clang's LibTooling to discorver potential refactor opportunities and potentially upgradable legacy code.

## Dependencies

Libraries:

```
clang
llvm-dev
libclang-dev
libffi-dev
libedit-dev
zlib1g-dev
libzstd-dev
libcurl4-openssl-dev
```

Recommended tools to build:

```
ninja-build
cmake
```

## Build

```bash
mkdir build
cmake -S . -B build -G Ninja # Ninja recommended for faster builds
cmake --build build
```

## Running

```
USAGE: unlegacyfier [options] <source0> [... <sourceN>] -- [options for clang]
```

For example this command runs the tool on `test.cpp` with the `enum-class-fixer` transformer enabled
with the `diff` frontend (prints the diff for each file to stdout). The underlying clang process will
run in standard conforming C++17 mode.

```bash
./build/unlegacyfier --enum-class-fixer --frontends=diff test_inputs/test.cpp -- --std=c++17
```