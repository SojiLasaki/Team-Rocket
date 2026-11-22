# Team Rocket Assembler

This repository contains early, separate pieces of a SIC/XE-style assembler. The passes are not yet connected by a single orchestrator.

## Requirements

- A C++17 compiler (`g++` or `clang++`). On macOS, `g++` usually invokes Apple Clang.
- The `nlohmann/json.hpp` header, required by `Pass_2/main.cpp`.

## Run Pass 2 object-program generation

Pass 2 currently reads `Pass_2/test.json` and writes the object program to `Pass_2/object_program.txt`.

```sh
cd Pass_2
g++ -std=c++17 main.cpp opcode_table.cpp -o assembler
./assembler
cat object_program.txt
```

The input must be a non-empty JSON array. Each instruction record needs `format`, `opCode`, `locationCounter`; format 3 and 4 records also need `n`, `i`, `x`, `b`, `p`, `e`, and `targetField`. Values are currently expected as strings, with addresses/opcodes/target fields represented in hexadecimal. Pass 2 emits SIC/XE Header (`H`), Text (`T`), and End (`E`) records. The sample input is `Pass_2/test.json`.

## Other existing programs

- `pass1/pass1.cpp` is a separate Pass 1 implementation. Its sample inputs and generated outputs are in `pass1/`.
- `intermediate/main.cpp` and `intermediate/helpers.cpp` contain intermediate-stage code.
- `pass2/main.cpp` is another Pass 2 implementation, separate from `Pass_2/`.
- `Pass_2/bin_to_hex.cpp` and `Pass_2/hex_to_bin.cpp` are standalone conversion utilities. Their included test script can be run from any directory:

```sh
./Pass_2/run_tests.sh
```

The root `main.cpp` is currently an experimental scratch program; it does not invoke the assembler passes. There is no root-level build command that runs the whole project end to end yet.

## Current progress

- Pass 1 source and sample input/output artifacts exist, but Pass 1 is not wired into Pass 2.
- Intermediate-stage source exists, but there is no agreed automated handoff to later stages.
- Pass 2 currently loads the sample JSON, generates object code for formats 1–4, and writes H/T/E records. It is an initial implementation and needs further validation, including address/length handling, format 2 register encoding, and correct instruction data for the header name and execution start address.
- Binary-to-hex and hex-to-binary utilities have a test script.
- A single main function that runs Pass 1, the intermediate stage, and Pass 2 has not been implemented.
