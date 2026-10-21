# woody_woodpacker

A small ELF64 binary packer. `woody_woodpacker` takes a 64-bit ELF executable,
encrypts its payload with AES, and prepends a stub that decrypts the original
program in memory at launch and runs it. The packed result is written to a new
file named `woody`.

This is a 42 school project.

## How it works

1. The input binary (the *host*) and a small *stub* are opened and validated as
   64-bit ELF executables of the same architecture.
2. A 128-bit AES key and IV are generated at random (or the key is supplied with
   `-k`), and the host payload is encrypted.
3. The stub is patched with the key/IV and the metadata it needs, then joined
   with the encrypted host to produce `woody`.
4. When `woody` runs, the stub decrypts the original program and transfers
   control to it.

## Build

Requires `clang`, `nasm`, and `make`.

```sh
make
```

This builds three artifacts:

- `woody_woodpacker` — the packer
- `stub` — the loader/decryptor that gets embedded
- `libft/libft.a` — the support library (built automatically)

Other targets: `make clean`, `make fclean`, `make re`.

## Usage

```sh
# Random key (printed is up to the stub; a key is generated for you)
./woody_woodpacker <path/to/elf64-binary>

# Supply your own 128-bit key as 32 hex characters
./woody_woodpacker -k 000102030405060708090a0b0c0d0e0f <path/to/elf64-binary>
```

The packed executable is written to `./woody` in the current directory. Run it
like any other program:

```sh
./woody
```

### Notes

- Only 64-bit ELF **executables** are supported; 32-bit inputs are rejected.
- The host and the stub must share the same target architecture.
- The output file is always `woody`; the input cannot itself be named `woody`.

## Layout

```
srcs/        packer sources (ELF parsing, section/segment handling, joining)
stub_srcs/   the stub: C entry point + AES encrypt/decrypt in assembly
includes/    headers
libft/       support library
Makefile
```
