# Coding Style in tlx

This file should document some of the C++ coding style used in tlx.

## 80 Columns Rule

All source must **strictly** adhere to the 80 column limit.

This may seem overly short considered today's wide high-resolution screens, but there are some good reasons.

- Sporadic long lines require more eye movement than a denser code packed for top-down reading.

- A shorter limit allow to arrange more than one column of code, e.g. for reading side-by-side or diffs.

For a fun read on the history of "Why 80 columns?", see  
<https://softwareengineering.stackexchange.com/questions/148677>

## Naming of Types, Methods, Functions, Variables.

- Styles: `ThisIsWrittenInCamelCase` or `this_is_written_in_snake_case`.

- All types, e.g. classes or typedef/usings are CamelCase, with the following exceptions:

  - All STL-style types which end with `_type`, `_traits`, or `iterator` are snake_case.
  
  - STL-style container or functor classes are granted an exception or may define an alias, e.g. `simple_vector`.
    
- All variables are written in snake_case. Attributes of classes have a tailing underscore_.

  - Some exceptions are granted, e.g. the `debug` variable cannot have a tailing underscore.

  - Constant variables may alternatively be written as `kCamelCase` without underscore.
  
  - Structs used mainly to store fields may optionally omit the tailing underscore on all members.
  
- Functions and class methods are written as snake_case.

  - currently no exceptions

## Formatting

- Ignore formatting while writing code. Uncrustify will be run and fix nearly all formatting.
