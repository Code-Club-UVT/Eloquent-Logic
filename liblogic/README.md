# LibLogic + LogicAgent
## Description
LogicAgent is the server component of Eloquent Logic. Its job is to perform all the specific algorithms and provide feedback to the frontend to be displayed.
This software is NOT to be used directly by end users. The following information is only relevant to developers wishing to build LogicAgent frontends or developers interested
in seeing the inner workings of LibLogic

## Requirements
- cmake (ver. 3.25 or higher)
- [conan](https://conan.io/downloads)
- C++23 capable compiler(tested against gcc13 and apple-clang 21 using macOS 27 Golden Gate SDK arm64)

## Building
Assuming you have all the dependencies installed (see section above for more information), building LogicAgent works as follows:

```bash
# all commands are run inside $PROJECT_DIR/liblogic
conan profile detect --force
conan install . --output-folder=build --build=missing
cd build
```
On Windows, then
```bash
cmake .. -G "Visual Studio 15 2017" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Release
```
On UNIX(-like)platforms
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Usage

The server may be started from the `logic_agent` executable. It expects input from **STDIN** and will return output on **STDOUT**. For exact definitions and sample queries, check the OpenRPC and JSON schemas from the [docs](docs) folder

The communication has been laid out in an JSONRPC 2.0 like fashion. After the server has received your request, it will start emitting feedback in JSON form until the final result is seen. The final result has the form of a JSON-RPC response

It is at the moment NOT possible to change the verbosity. Clients MAY ignore unnecessary feedback emitted by the server.

Should the client wish to end its session, it SHOULD call the `shutdown` RPC method. Exception being if the server has hung up. In that case, the client MAY forcefully terminate the server.

For more information on the supported commands, see the [docs](docs/) directory.

## Implementation

This project has been split into two main projects:
1. The LibLogic Library
2. The Logic Agent library frontend


### LibLogic Library

The LibLogic library is split into multiple subtargets that specialize on a particular subcomponent of the main library, such as parsing, AST transformation or SAT solving.
LibLogic also offers a series of feedback classes that allow a program that uses the library to provide detailed feedback on each algorithm implemented, enabling a clear
description of the algorithms suitable in a classroom environment. 

### LibLogic Modules

LibLogic is split into the following modules and submodules:

- Textual Analysis
  - Lexer
  - Strict Parser
  - Relaxed Parser
  - Syntax Tree
- Syntax Transformation Module
- SAT solver
  - resolution
  - DP
  - DPLL (RAND heuristic)
  - DPLL (Most Common Literal heuristic)
- Logical consequence solver

For more information on each of the modules, consult the appropriate documentation in [docs/modules](docs/modules). 
Each module may be linked against separately for users who wish to have a smaller program footprint, although it is not recommended


### Logic Agent
The Logic Agent is the main communication method for a client to utilize the algorithms implemented by the library. The communication protocol is inspired by the Language Server Protocol
used by many 

The Agent, in spite of its name, does not rely on LLM technology for its duties. Like LibLogic, it is split into multiple parts:

- Main Dispatcher
- RPC method calls
- Data Transmission Objects (DTOs)



## Further reading
More information can be found in the [docs](docs/) directory
