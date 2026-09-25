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

## Feature-3: Report Questions

**Q: Compare the Makefile from Part 2 and Part 3. What are the key differences?**

Part 2's rule linked all object files directly: `$(TARGET): $(OBJECTS)` followed by `$(CC) $(OBJECTS) -o $(TARGET)`. Part 3 introduces a new intermediate step: object files from the library modules are first archived into `libmyutils.a` using `ar`, and only then is the driver's `main.o` linked against that archive using `-L` (library search path) and `-l` (library name) flags, instead of listing object files individually.

**Q: What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?**

`ar` creates and manages archive files — bundling multiple object files into a single `.a` static library. `ranlib` generates or updates a symbol index embedded in the archive so the linker can quickly locate which object file defines a given symbol, without scanning every object file's contents. Using `ar` with the `s` flag (`rcs`) performs this indexing automatically, making a separate `ranlib` call unnecessary.

**Q: When you run `nm` on `client_static`, are symbols like `mystrlen` present? What does this tell you?**

Yes, `mystrlen` and the other library functions appear in `nm`'s output for `client_static`. This confirms that static linking physically copies the machine code for each used function directly into the final executable at link time — the executable is fully self-contained and does not depend on `libmyutils.a` being present at runtime.
