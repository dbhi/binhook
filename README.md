**binhook** is a survey of techniques to hook and/or replace functions in executable binaries or shared libraries. These allow to change the behaviour of programs, without requiring access to source code and without recompilation.

Given a pre-built application ([app](src/app.c)) that uses a function ([func](src/func.c)) with a known signature (e.g. `char* func(char* A, char* B, uint32_t length)`), the target of these techniques is to execute an alternative implementation defined in independent C sources ([replacement](src/replacement.c)).

# dynamic/runtime modification

## [ld_preload](tests/ld_preload/)

> supported os (?) platforms (any)

`LD_PRELOAD` is an environment variable supported by the dynamic linker on GNU/Linux systems.

> http://man7.org/linux/man-pages/man8/ld.so.8.html
>
> A list of additional, user-specified, ELF shared objects to be loaded before all others.
> This feature can be used to selectively override functions in other shared objects.

```
             +------------+              +-------------+  +--------------+
             |    app     |              | patched_app |  |    app_lib   |
             | +--------+ |              | +---------+ |  | +----------+ |
             | | func.o | +-----+        | | repl.o  | |  | |libfunc.so| +-----+
             | +--------+ |     |        | +---------+ |  | +----------+ |     |
             +------------+     |        +-------------+  +--------------+     |
                   |            v               |                |             v
                   |       +----+-----+         |                |        +----+-----+
LD_PRELOAD:        |       |librepl.so|         |                |        |librepl.so|
                   |       +----+-----+         |                |        +----+-----+
                   v            v               v                v             v
    output:     default      default       replacement        default      replacement
```

In this test suite, the following artifacts are built:

- `[func|replacement][.o|.so]`: both the original and the replacement function are independently built as objects and as shared libraries.
- `app`: `app.c` and `func.o`.
- `app_lib`: `app.c` and `func.so`.
- `patched_app`: `app.c` and `replacement.o`.

And the following tests are executed:

- `app` without `LD_PRELOAD`. Regular execution of the app with built in `func`.
- `app` with `LD_PRELOAD`. When the function is built in the app, `LD_PRELOAD` has no effect at all.
- `patched_app` without `LD_PRELOAD`. Execution of the patched app, should the user have access to app sources to built it.
- `app_lib` without `LD_PRELOAD`. Regular execution of the app with `func` loaded from a shared lib.
- `app_lib` with `LD_PRELOAD`. Execution of the `replacement` function, since `replacement.so` is loaded before `libfunc.so`.

Notes:

- `LD_PRELOAD` allows to easily replace functions that the app uses from shared libraries. However, it is not suitable for functions that are built in the app. By the same token, it is not suitable for statically compiled bineries.
- Both the app and/or the shared libraries can be built with `gcc -Os -s`. No additional symbol info is required.

## Hook by Import Address Table (IAT) / Procedure Linkage Table (PLT) patching

### [elf_hook](tests/elf_hook/)

> supported os (?) platforms (?)

In this testsuite, library [shoumikhin/ELF-Hook](https://github.com/shoumikhin/ELF-Hook) is used. ELF-Hook allows to replace a function which is called from another function defined in a shared library.

```
       A/B                           C/D

+------+ +------+             +------+ +------+ +---+
|hook.c| |test.c|             |hook.c| |test.c| |app|
+-----++ ++-----+             +-----++ ++-----+ +-+-+
      |   |                         |   |         |
      >-+-<                         >-+-<         |rename
  build |                       build |           v
        v     +----------+            v     +-----+----+
       test <-+libfunc.so|           test <-+libfunc.so|
              +----------+                  +----------+

```

The following artifacts are built:

- `libfunc.so`: the target ([func](src/func.c)) is wrapped ([wrapfunc](src/wrapfunc.c)).
- `testA`: `libfunc.so` is loaded with `dlopen` and `get_module_base_address` and `elf_hook` are used to replace `func` with `hook` (defined in [testA](tests/elf_hook/testA.c)).
- `testB`: same as `testA`, but `dlsym` is used to get a pointer to the entrypoint.
- `testC`: same as `testB`, but an exebutable is loaded instead of a shared library.
- `testD`: same as `testC`, but the entrypoint is `main` (from the loaded app).

And the following tests are executed:

- `testA`: `wrapfunc` is executed before and after calling `elf_hook`.
- `testB`: `wrapfunc` is executed before and after calling `elf_hook`.
- `testC`: `func` is executed before calling `elf_hook`. Setting the redirect fails.
- `testD`: `main` is execute before calling `elf_hook`. Setting the redirect fails.

Notes:

- Compared to LD_PRELOAD, ELF-Hook allows to apply modifications to a single shared library.
- Replacing a function in a shared library is supported (testA, testB), but the same approach fails with a PIE executable (testC, testD).
- ELF-Hook allows to optionally execute the original function from inside the hook. Hence, `wrapfunc` is the entrypoint, and whenever `func` is used, `hook` can execute instructions before and/or after.

### [kubo/plthook](https://github.com/kubo/plthook)

> supported os (GNU/Linux, Windows, macOS, solaris, FreeBSD) platforms (x64, x86, arm, aarch64, powerpc, powerpc64le)

to do...

## Hook by JMP instruction insertion

### [kubo/funchook](https://github.com/kubo/funchook)

> supported os (GNU/Linux, Windows, macOS) platforms (x64, x86)

to do...

- [List of API Hook Libraries](https://github.com/kubo/funchook/wiki/List-of-API-Hook-Libraries)
- [x86 API Hooking Demystified](http://jbremer.org/x86-api-hooking-demystified/) by Jurriaan Bremer

## Dynamic Binary Modification (DBM)

### MAMBO

> supported os (GNU/Linux) platforms (armv7, aarch32, aarch64)

work in progress...

### DynamoRIO

> supported os (?) platforms (?)

work in progress...

# static/file modification

## [GNU poke](http://www.jemarch.net/poke.html)

> supported os (?) platforms (?)

work in progress... the extensible editor for structured binary data

## [lief-project/LIEF](https://github.com/lief-project/LIEF)

> supported os (?) platforms (?)

to do... LIEF - Library to Instrument Executable Formats

- https://lief.quarkslab.com/
- https://2018.pass-the-salt.org/files/talks/03-static-instrumentation.pdf

# injection into a running process

## [kubo/injector](https://github.com/kubo/injector)

> supported os (GNU/Linux, Windows) platforms (x64, arm64, ?)

to do...

# references

- Fixing/Making Holes in Binaries by Shaun Clowes ([slides](https://www.blackhat.com/presentations/bh-asia-02/Clowes/bh-asia-02-clowes.pdf), [video](https://www.youtube.com/watch?v=18DKETYfvjg))
- [Dynamic linker tricks: Using LD_PRELOAD to cheat, inject features and investigate programs](https://rafalcieslak.wordpress.com/2013/04/02/dynamic-linker-tricks-using-ld_preload-to-cheat-inject-features-and-investigate-programs/) by Rafał Cieślak
- [Redirecting Functions in Shared ELF Libraries](https://www.codeproject.com/Articles/70302/Redirecting-functions-in-shared-ELF-libraries) by [Anthony Shoumikhin](https://github.com/shoumikhin) ([shoumikhin/ELF-Hook](https://github.com/shoumikhin/ELF-Hook))
- [cea-sec/miasm](https://github.com/cea-sec/miasm)
- [s3team/uroboros](https://github.com/s3team/uroboros)
- [PEBIL: Static Binary Instrumentation for x86/Linux](https://www.sdsc.edu/pmac/tools/pebil.html)
- [asciiflow.com](http://asciiflow.com/)
