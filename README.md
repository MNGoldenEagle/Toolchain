# Toolchain
A command-line toolchain for creating overlays and other compiled code in compatible N64 titles.
This toolchain is available as a cross-platform release using .NET Standard 2.0 and open-source tools.
All releases include compiled binaries for the toolchain and its dependencies.

Note that each mode is documented in terms of how it operates and uses its dependencies.  If you want to
replace part or all of Toolchain with the original tools, you may use the documentation as a starting
point for what flags will work for your pipeline.  If you are interested in getting started on building
overlays as quickly as possible, start by reading the first two modes in the Usage section.

* [Dependencies](#dependencies)
* [Usage](#usage)
  - [PrepareLinker Mode](#preparelinker-mode)
  - [Build Mode](#build-mode)
  - [Compile Mode](#compile-mode)
  - [Assembler Mode](#assembler-mode)
  - [Linker Mode](#linker-mode)
  - [Overlay Mode](#overlay-mode)
  - [Pj64Sym Mode](#pj64sym-mode)
* [FAQ](#faq)
* [Contributions](#contributions)

## Dependencies
The Toolchain relies on Clang/LLVM for compiling C code into assembly, as it typically has useful error
messages and more reliable behavior compared to GCC.  Toolchain releases come with a precompiled version
of Clang by default that supports cross-compilation.

The Toolchain also relies on GNU binutils for MIPS to create ELF files and perform all necessary linking.
Precompiled versions of binutils are packaged with the releases by default.

## Usage
Toolchain offers a number of different "modes" of operation, depending on what files you want to operate
on. You can modify the files after each mode operates to make manual adjustments to the files as desired,
which may be desired if you wish to customize your assembled code or modify how it performs linking.

Note that Toolchain comes with expressive help at the command-line that explains all of the options and
flags available for each mode.  If you use `toolchain --help` or `toolchain /?` it will display a list
of accepted modes.

### PrepareLinker Mode
```toolchain preparelinker```

Generates linker scripts for all possible targets by parsing the `Memory Table.csv` file and writing the
entries into the templates folder.  This step is typically not necessary; you only need to execute this if
you have modified the included header files and added the corresponding entry in the CSV.

The format of the CSV file is documented in the header row.  You can modify the file using a program such
as Excel or LibreOffice.  Note that the CSV will likely be replaced with a different mechanism in a future
release.

### Build Mode
```toolchain build --output=<overlay file> [--target=version] [--release] <source files>```

Builds a compiled, linked, injectable overlay file based on the provided source files.  This mode is provided
as a convenience to easily generate overlays without invoking each of the individual modes manually.  Only a
small subset of options are made available using this mode.  For more complex build operations, invoke each
mode manually.

Note that this is provided primarily as a convenience.  For larger projects, you may want to leverage a
build automation tool like [cmake](https://cmake.org).

For more information about the `--target` option, see the documentation for the [Link Mode](#link-mode).

The `--release` argument indicates that the overlay is in a releasable state and should be optimized as
aggressively as possible.  In addition, the `RELEASE` macro will be defined during the compilation step.
When this argument is not specified, no optimizations will be performed and the `DEBUG` macro will be
defined instead during the compilation step.

### Compile Mode
```toolchain [-O#] <source files>```

Compiles the provided source (.c) files into an assembled (.S) file in the MIPS dialect.  Header files for the
game engine are automatically included as system headers (i.e., files that are included via
`#include <filename.h>`).  The headers are included as part of the release.  If you are using an IDE,
you will want to make sure it scans the `headers/` path in the installation folder for include files.

The `-O` or `--optimize` argument allows you to specify the level of optimization to perform on the files.
If you specify a level of 0, the Toolchain will automatically define the `DEBUG` macro which can be used
in your code.  Otherwise, it will define the `RELEASE` macro.  You can use this to include additional logic
for debugging and assertion purposes.

The Toolchain will automatically run Clang using the best options possible to match those used by existing
N64 titles.  Specifically, it compiles with the following options:

|Option|Reason|
|------|------|
|fno-addrsig|Disables generation of the address significance table, which isn't supported by binutils.|
|fomit-frame-pointer|Does not use $S8 register as a frame pointer.|
|DF3DEX_GBI_2|Use F3DEX2 version of the Graphics Binary Interface.|
|D_LANGUAGE_C|Compiling for C code.|
|v|Verbose output for debugging purposes.|
|target mips-mips2-elf<sup>1</sup>|Emits assembly for the MIPS II ISA intended for ELF binaries.|
|march=mips2<sup>1</sup>|Target the MIPS II architecture.  This is necessary to use the `o32` ABI.|
|mabi=o32<sup>1</sup>|Use the `o32` MIPS ABI.  All existing N64 titles use this ABI.|
|mfp64<sup>1</sup>|Enables 64-bit floating point registers.  This emulates MIPS III functionality.|
|S|Compile to the assembly stage.  This is due to limitations with the Clang linker.|

1: Clang forces use the `n32` or `n64` ABI when compiling on MIPS III or later, however the vast majority
of titles for the N64 use the `o32` ABI.  Since the `o32` and `n32` ABIs are not compatible with each other
(due to the differing number of argument registers), this combination of flags still properly compiles source
files for MIPS III without breaking existing compatibility while still supporting 64-bit floating point
registers.  This will be fixed when the Clang backend has improved.

### Assembler Mode
```toolchain assemble --output=<ELF file> <source files>```

Assembles the provided assembled files (.S) into ELF files.  The generated ELF file is not linked, so any
symbols used from the game engine will not be resolved at this point.

The `-o` or `--output` argument allows you to specify the name of the generated ELF file.  This argument
is required.

The Toolchain will automatically run GNU ASsembler using the following options:

|Option|Reason|
|------|------|
|march mips3|Generates opcodes supported by MIPS III processors.|
|mabi 32|Ensures generated opcodes conform to the `o32` ISA.|
|mno-shared|Disable position-dependent optimizations.|
|call_nonpic|Generates non-PIC code for dynamic shared objects.|
|EB|Generate Big Endian output.|

### Linker Mode
```toolchain link --output=<ELF file> [--target=version] <object files to link>```

Links the specified ELF files together into a new ELF file with all external symbols resolved for the
specified target.  By default, the Toolchain will resolve symbols against the PAL Debug ROM for Ocarina
of Time: Master Quest.  Accepted versions for the `-t` or `--target` parameter are:

|Value|Description|
|-----|-----------|
|1.0U<sup>1, 2</sup>|Ocarina of Time NTSC 1.0 (US/Japan)|
|1.1U<sup>1</sup>|Ocarina of Time NTSC 1.1 (US/Japan)|
|1.2U<sup>1</sup>|Ocarina of Time NTSC 1.2 (US/Japan)|
|1.0E|Ocarina of Time PAL 1.0 (EU)|
|1.1E|Ocarina of Time PAL 1.1 (EU)|
|MQU|Ocarina of Time: Master Quest NTSC|
|DEBUG<sup>2</sup>|Ocarina of Time: Master Quest Debug ROM PAL|

1: You may also specify a J postfix instead of U.  Toolchain treats them as equivalent.

2: Toolchain only generates linker files for 1.0U and DEBUG by default. See `preparelink` command for
more details.

The `link` command will automatically run the GNU Linker with the following arguments:

|Option|Reason|
|------|------|
|A elf32-bigmips|Generates a 32-bit ELF file in big endian for MIPS.|
|S|Strips debugging symbols from the resulting file.|
|T _\<file\>_|Uses the linker script to resolve symbols and define the appropriate memory space.|

### Overlay Mode
```toolchain overlay --output=<actor file>  [--init=<symbol name>] [--fno-program] [--show-init-addr] [--porcelain] <ELF object file>```

Generates an overlay compatible with the Zelda 64 game engine using the provided ELF object file linked by
the `link` command.  The overlay can then be inserted into an Ocarina of Time or Majora's Mask ROM image.

By default, the `overlay` command assumes that you are generating an actor overlay, which needs to emit
the address of the actor's initialization variable to set up the actor table appropriately.  This variable
is assumed to be called `INIT` by default, but you can override this name using the `--init` option.  Note
that most compilers will elide the variable if it's not referenced in your code anywhere; to prevent this
from happening, use `__attribute__((used))` on your variable spec (this works for Clang and GCC).
For example:

```c
static const ActorInit INIT __attribute__((used)) = {
    .actorID = 0x72,
    .actorType = ACT_TYP_PROP2,
    .roomNumber = ACT_RM_UNDEF,
    .flags = 0,
    .objectID = 1,
    .instanceSize = sizeof(DemoActor),
    .constructor = constructor,
    .destructor = destructor,
    .update = update,
    .draw = draw
};
```

If your overlay consists purely of static resources (like precompiled display lists, strings, constants) and
no code, use the `--fno-program` flag to indicate that there is no executable code, otherwise Toolchain
will raise an exception.

If you want to update the actor table with the location of the initialization variable, specify the
`--show-init-addr` to output the VRAM location of the initialization variable within the overlay.  If the
output is intended for a program to consume, pass in the `--porcelain` flag and it will write to standard
out in an easily consumable manner.

For example, if the actor is being written to `DemoActor.zactor` with an initialization variable called
InitVar:

|No `--porcelain`|`--porcelain`|
|------------------|---------------|
|```Overlay written to DemoActor.zactor.```<br/>```InitVar structure located at 80F00FF0.```|```DemoActor.zactor```<br/>```80F00FF0```

Overlay is implemented directly in the Toolchain and does not rely on any external programs.

### Pj64Sym Mode
```toolchain pj64sym --target=<version> --output=<symbol file>```

Generates a .sym file compatible with the Project64 debugger based on the symbols currently defined in the
memory table.  Due to limitations in the memory table, all symbols will be exported as "data" with a
description matching their category.

For more information about the `--target` option, see the documentation for the [Link Mode](#link-mode).

## FAQ
### What's the difference between this and the z64ovl tool?
The z64ovl tool and this tool were developed in parallel, so there is quite a bit of overlap between the
two tools.

Currently, the only major difference is how your code is linked to the ROM.  z64ovl uses inline assembly and
version-specific C files to link function calls to the existing code in the game.  Toolchain uses a linker
script to link the function calls to the existing code.

The consequences of this are minor but important.  With inline assembly, it is more difficult to maintain
or modify when a symbol moves location, but you will get immediate feedback from the compiler when using a
symbol that doesn't exist.  With a linker script, it's easier to maintain multiple versions and move symbols
to other locations, but you will not see an error message until you attempt to link your code.

### What's the deal with this Memory Table CSV file?
It's a way for the program to keep track of where functions are located across each version of the ROM.  The
values are provided based on what has been researched by the community so far, which mostly spans versions
1.0U and Master Quest Debug ROM for Ocarina of Time.  Feel free to modify this file if you need to introduce
new functions or define them for a version that doesn't currently include them.  And if you do come up with
any, please contribute them to the project!

In the future, the memory table file will be replaced with a new mechanism that's less reliant on 70's tech.

### Why does this include DLL files?
The .NET Framework generates .DLL files for each assembly, regardless of the target operating system.  They
are the equivalent of .so files, though they do not conform to the ELF object format.  In the future, the
application will be statically compiled once its reliance on reflection is removed.

### Why clang instead of GCC?
GCC had some compilation issues, particularly with the GBI, but likely would work fine for most projects.
Clang has much better error messaging and is more user-friendly, but is not as efficient when generating
assembly.

A future improvement to this tool will be allowing the choice of compiler.

## Contributions
To contribute to Toolchain or the accompanying SDK, create an issue and pull request on
https://github.com/MNGoldenEagle/Toolchain.  Please license your contributions in a manner compatible with
this project.
