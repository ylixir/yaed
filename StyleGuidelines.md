# Naming conventions #
The naming conventions are here to allow you to know what the token refers to without looking it up. You can tell types from functions from constants from variables, etc.
## Constants ##
Constants are declared in all caps, words separated by underlines
  * `MAX_RANGE`
## Variables ##
Variables are declared with the first character lowercase.  Multiple words are separated by the underscore character.  Variables are ALWAYS descriptive. This means that 99% of the time they are words not letters.
  * Exceptions are things like: `x`, `y`, `z`, `i`, `n`.
  * `int multi_word_variable;`
## Functions ##
Function names are declared with every first letter of every word but the first as uppercase.  Every other letter including the first is lowercase. No underscore separates words.
  * `void sampleFunction()`
## Types ##
Types are declared similarly to functions, but the first letter is capitalized.
  * `struct SampleType`

## Modules ##
Modules, aka, source and/or header pairs are all lower case. Words are separated by hyphens.
  * `main.c`
  * `source-view.h`, `source-view.c`

# Coding Conventions #
These conventions aim to increase readability, and decrease typo related bugs.
## Comparison Statements ##
Literals and constants are on the left side of expressions whenever possible. This is mostly because of the idiotic assignment/comparison semantics in C that lead to silent errors.
  * `if(NULL == pointer)`
    * prevents `if(pointer = NULL)` which always returns `false` and overwrites the variable while it's at it.
    * prevents `if(variable = 3)` which always returns `true` and overwrites the variable while it's at it.
  * `while(0<variable)`
    * keeps one in the habit for when the above case comes up
## Block layout ##
Layout of code is important as mixing layouts makes code harder to read.
  * Curly braces should be on their own line
```
while(1)
{
}
```

  * Code should not extend past the 80 character mark on one line. This will make it easier for people editing from smaller windows to work.
  * Declarations (functions, loops, etc) should be on one line as long as the above two conditions can be met.
  * Indentation should be with space characters, not tab characters. Use an indent of 2 spaces.
  * Don't end empty loops with a semicolon.  Use two curly braces instead.
```
for(int i=0;i<1000;i++){}
```
# Typing #
Types are important.  C types allow you to play fast and loose.  Try to avoid this. Rules for choosing a type:
  1. The type should represent the data it contains, even if that means more casting about to interface with GTK.
  1. Casting is bad, if you don't have a good reason to do it, don't.

## Specific type rules ##
### `size_t` ###
If an unsigned variable has to span the address space, use `size_t`. An example of this is an array index.
### `ptrdiff_t` ###
If a signed variable has to span the address space, use `ptrdiff_t`. These are mostly results of pointer arithmetic, etc.
### Integer types ###
Avoid `short`, `long`, and `gint`. These modifiers aren't useful these days. All they really say is that they are integers, so use `int` instead. If a specific number of bits is required use the appropriate glib types.
### Character types ###
Think hard about `char`. `char` would imply character data, but really it means 8 bit int that is easy to use as an ascii value.  maybe you should be thinking about unicode. `char` should only be used to reference strings, not single characters.
### Boolean types ###
Include `stdbool.h`. Use `bool` when you are dealing with boolean data.  Bitmasks are excluded of course.
### 64 bit integers ###
Use `long long` if you need a minimum of 64 bits. This is obviously an integer type, do not cast back and forth with pointers.

# Optimization #
When in doubt optimize for 64 bits or better.  Most machines out there now are running 64 bit operating systems.

# Endianness #
DO NOT ASSUME ENDIANNESS. Arm is big endian.  PPC is big endian.  x86 is little endian. Who knows what playstation/xbox/wii/zune/random platform are. this should come up rarely, but think about it whenever accessing a file or network etc.