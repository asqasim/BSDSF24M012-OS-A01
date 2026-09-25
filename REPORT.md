## Feature-2: Report Questions

**Q: Explain the linking rule `$(TARGET): $(OBJECTS)` in this part's Makefile. How does it differ from a Makefile rule that links against a library?**

This rule tells `make` that to build `$(TARGET)` (the `client` executable), all files listed in `$(OBJECTS)` must exist and be up to date first. When triggered, `gcc` links these object files directly, bundling their compiled code straight into the final executable.

This differs from linking against a library (e.g. `-lmyutils`), where instead of listing individual `.o` files, the linker is pointed at a pre-bundled archive (`.a`) or shared object (`.so`) using the `-L` (library path) and `-l` (library name) flags. The linker then pulls in only the specific functions it needs from that library, rather than every object file being explicitly named in the rule.

---

**Q: What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?**

A git tag is a fixed pointer to one specific commit. Unlike a branch, a tag never moves forward as new commits are added, which makes it useful for marking release points — for example, "this exact commit is version 0.1.1" — so that state can always be found again later.

- A **lightweight (simple) tag** (`git tag v1.0`) is just a name pointing at a commit, with no extra metadata.
- An **annotated tag** (`git tag -a`) is a full git object with its own message, author, and timestamp. Annotated tags are recommended for releases since they carry more information, and they're what GitHub Releases are built on top of.

---

**Q: What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?**

A GitHub Release wraps a tag with human-readable release notes, making it easy to identify and distribute a specific version of the project.

Attaching compiled binaries (like `bin/client`) lets users download and run the program directly without needing to clone the repository and compile it themselves — useful for end users who just want the software, not the source code.

---

## Feature-3: Report Questions

**Q: Compare the Makefile from Part 2 and Part 3. What are the key differences?**

Part 2's rule linked all object files directly: `$(TARGET): $(OBJECTS)` followed by `$(CC) $(OBJECTS) -o $(TARGET)`. Part 3 introduces a new intermediate step: object files from the library modules are first archived into `libmyutils.a` using `ar`, and only then is the driver's `main.o` linked against that archive using `-L` (library search path) and `-l` (library name) flags, instead of listing object files individually.

**Q: What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?**

`ar` creates and manages archive files — bundling multiple object files into a single `.a` static library. `ranlib` generates or updates a symbol index embedded in the archive so the linker can quickly locate which object file defines a given symbol, without scanning every object file's contents. Using `ar` with the `s` flag (`rcs`) performs this indexing automatically, making a separate `ranlib` call unnecessary.

**Q: When you run `nm` on `client_static`, are symbols like `mystrlen` present? What does this tell you?**

Yes, `mystrlen` and the other library functions appear in `nm`'s output for `client_static`. This confirms that static linking physically copies the machine code for each used function directly into the final executable at link time — the executable is fully self-contained and does not depend on `libmyutils.a` being present at runtime.

---

## Feature-4: Report Questions

**Q: What is Position-Independent Code (-fPIC) and why is it a fundamental requirement for creating shared libraries?**

Position-Independent Code is machine code that can execute correctly no matter where in memory it gets loaded. Normally, compiled code contains hard-coded memory addresses for its functions and global data. A shared library (`.so`), however, is loaded into a different process's address space at runtime, and that address can vary between programs or even between runs of the same program (especially with ASLR enabled). If the library's code contained fixed addresses, it would break as soon as it wasn't loaded at the exact address it was compiled for.

`-fPIC` solves this by making the compiler generate code that accesses functions and data through offsets and indirect tables (the GOT and PLT) instead of absolute addresses. This is why the Makefile compiles a separate set of `_pic.o` object files only for the `.so` build — the static library doesn't need this indirection since its code is copied directly into the final executable at a fixed, known location.

**Q: Explain the difference in file size between your static and dynamic clients. Why does this difference exist?**

In my case, both binaries came out to the same size (17K):

```
-rwxr-xr-x 1 qasim qasim 17K Sep 25 21:56 bin/client_dynamic
-rwxr-xr-x 1 qasim qasim 17K Sep 25 21:56 bin/client_static
```

Normally, a statically linked executable is larger because the library's compiled code is physically copied into the binary, while a dynamically linked one only stores a reference (an undefined symbol) that gets resolved at runtime — confirmed by comparing `nm` output on both:

```
$ nm bin/client_static | grep mystrlen
000000000000159a T mystrlen      # defined — code is embedded

$ nm bin/client_dynamic | grep mystrlen
                 U mystrlen      # undefined — resolved at runtime
```

The reason the overall file sizes don't differ here is that `libmyutils` is a very small library — only a handful of simple string/file functions, amounting to a few hundred bytes of actual code. Both executables are still dynamically linked against `libc` and `ld-linux` regardless (confirmed via `ldd`), and that shared overhead — ELF headers, dynamic symbol tables, PLT/GOT stubs — dominates the file size at this scale. Because Linux also page-aligns sections (typically to 4KB boundaries), a difference of a few hundred bytes in the actual library code often doesn't even show up in the rounded file size. This size gap becomes significant only with much larger libraries (e.g. `libssl`, `libcurl`), where static linking would pull in hundreds of KB to MB of extra code.

**Q: What is the LD_LIBRARY_PATH environment variable? Why was it necessary to set it for your program to run, and what does this tell you about the responsibilities of the operating system's dynamic loader?**

`LD_LIBRARY_PATH` is an environment variable that tells the dynamic loader (`ld-linux.so`) additional directories to search when resolving shared library dependencies at program startup.

Before setting it, running `client_dynamic` failed because `libmyutils.so` lives in a custom project directory (`lib/`) that isn't part of the loader's default search path (which normally includes system directories like `/lib`, `/usr/lib`, and paths listed in `/etc/ld.so.conf`). This was confirmed directly:

```
$ ldd bin/client_dynamic
        libmyutils.so => not found
```

After exporting `LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH`, the loader could find and resolve it, and `ldd` showed the library correctly linked to the path inside my project's `lib/` directory.

This demonstrates that with dynamic linking, resolving dependencies is **not** the compiler or linker's job at build time — it's deferred entirely to the OS's dynamic loader, which runs every time the program starts. The loader is responsible for locating, loading, and binding every shared library a program depends on at launch, which is why a dynamically linked program is not truly "complete" on its own — it depends on its runtime environment being configured correctly, unlike a static binary which is fully self-contained.

---

## Feature-5: Report Questions

**Q: What is the purpose of a man page, and what do the standard sections (.TH, .SH NAME, .SH SYNOPSIS, .SH DESCRIPTION) represent?**

A man page is Linux's standard, built-in documentation format for commands, library functions, and system calls, viewable directly from the terminal via the `man` command without needing internet access. It's written in `groff`, a markup language that `man` renders into formatted terminal output.

- `.TH` (Title Header) declares the page's title, the man section number, the release date, and the source/version — this is metadata `man` uses to catalog and locate the page.
- `.SH NAME` gives the command/function name and a one-line summary, which is also what `apropos`/`man -k` searches against.
- `.SH SYNOPSIS` shows the exact calling syntax — for a function, its signature and required header; for a command, its usage pattern and flags.
- `.SH DESCRIPTION` is the detailed explanation of behavior, parameters, and edge cases.

**Q: Why did you install the man pages into man1 instead of man3, even though the assignment's directory structure used man/man3?**

Man sections group documentation by category: section 1 is for user-executable commands, while section 3 is for C library functions referenced via `#include`. Although the project's `man/man3/` directory (as specified in the assignment) holds pages for both `client` (a command) and individual library functions like `mystrlen`, when installing system-wide I placed all of them into `/usr/local/share/man/man1` because in this project they are all invoked/documented as command-line-accessible references rather than being installed as a linkable public API with its own dedicated section-3 documentation set. This is a simplification made for this assignment; a fully rigorous package would split function-level docs into section 3 and only the `client` command into section 1.

**Q: What does the `install` target in your Makefile do, and why is registering the library path with ldconfig necessary?**

The `install` target copies the compiled `client` executable into `/usr/local/bin` (a directory already on the system `PATH`) and the man pages into the system man directory, so both become accessible from anywhere without needing to `cd` into the project folder.

Registering `lib/` in `/etc/ld.so.conf.d/` and running `ldconfig` adds the project's library directory to the dynamic loader's system-wide cache of trusted library search paths. Without this step, running the installed `client` binary from outside the project directory would still fail with "cannot open shared object file," since `LD_LIBRARY_PATH` is a per-shell-session variable and wouldn't apply globally. `ldconfig` makes the shared library discoverable system-wide, permanently, regardless of which shell or user runs the program.
