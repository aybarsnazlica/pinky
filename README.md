# pinky

A [Pinky language](https://pinky-lang.org) interpreter implemented in C++.

## Build and Install

Install without administrative privileges by using a user-local prefix.

```shell
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
cmake --install build
```

Make sure your shell can find user-local binaries:

```shell
export PATH="$HOME/.local/bin:$PATH"
```

To make the `PATH` change persistent in `zsh`, add that line to `~/.zshrc`.

## Usage

```shell
pinky [--verbose] <filename>
```
