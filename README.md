# ti-basic-compiler📝
  
ti-basic-compiler is a compiler for ti-basic. It's designed to be better than ti-connect, and as such it can be used as a replacement for the ti-connect ""IDE"". The output .8xp file is fully compatible with the TI calculators that support ti-basic. We have tried to model the original language as much as we could, but they are a few quirks, mostly in the function and variable names. Use `src/functions.h` as a reference for all the identifiers.

## Build

**For Unix and mingw/cygwin users:**
1. Clone this repo: `git clone https://github.com/stickyPiston/ti-basic-compiler.git`
2. Run `make`, it will create a folder with a binary called `tic`. If you want to use the compiler globally run `make install`. Because `tic` is already used in Unix-like operating systems you need to run `ti-compiler`.
3. Compile the example ti-basic file: `./bin/tic example.ti`
4. Open tic's help manual by running `./bin/tic -h`

**For Windows (msvc) users:**
good luck (future support is planned, but not prioritised)
