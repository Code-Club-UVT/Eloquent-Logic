# Project description

This is a library that contains a series of algorithms for parsing and doing some common operations in propositional logic.

# Structure
The main folders of interest are:
- src/ -> source code of library
- logic_agent -> place for the CLI frontend
- tests/ -> tests for the library

Other folders may be generated from the build process and do not present particular interest.


## Directory structure
src/common -> base target with a feedback listener class
src/parser -> lexer + two kinds of parsers: a cursor based one as strict_parser and a PRATT one as relaxed_parser
src/sat -> SAT algorithm library
src/transformations -> AST transformations according to propositional logic expression simplification rules

## CMake Target structures:
- most folders inside src/ contain files pertaining to one general feature of the library
- those features are individually put inside their own CMake library target
- this project uses Conan for dependency management.
- this project uses GTest for tests

## Dos and don'ts
- when writing tests, place the .cpp files inside subfolders of tests/ that describe their overall feature being tested, mirroring the structure in src/
- DON'T link the entire library (liblogic) to the individual tests
- DO put multiple test files pertaining to a particular feature inside their own subfolder
- DO make use of the listeners placed inside functions
- DO make use of test fixtures
- When changing the schemas for remote procedure calls, DO ensure that all changes are reflected in the schema tests