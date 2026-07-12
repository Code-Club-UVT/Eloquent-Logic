# LibLogic
## Note on refactoring

This is a branch dedicated to performing some structural refactoring to the structure of this library.
We are aiming to introduce a JSON-RPC compliant server structure for greater flexibility when working with different
programming language ecosystems.

All information about planning can be found inside `docs/refactoring`
## Cloning
LibLogic requires the ANTLR C++ runtime, which is included as a submodule:

`git clone --recurse-submodules https://github.com/EloquentDevTeam/LibLogic.git`

If you accidentally forgot to include `--recurse-submodules`, you can run:

`git submodule update --init`
