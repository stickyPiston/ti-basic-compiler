# ti-basic-compiler📝
  
ti-basic-compiler is a compiler for ti-basic. It's designed to be better than ti-connect, and as such it can be used as a replacement for the ti-connect ""IDE"". The output .8xp file is fully compatible with the TI calculators that support ti-basic. We have tried to model the original language as much as we could, but they are a few quirks, mostly in the function and variable names. Use `src/functions.h` as a reference for all the identifiers.

## Build

**For Unix and mingw/cygwin users:**
1. Clone this repo: `git clone https://github.com/stickyPiston/ti-basic-compiler.git`
2. Run `make`, it will output a binary called `tic`
3. Compile the example ti-basic file: `./tic example.ti`

**For Windows (msvc) users:**
good luck
