# PLC Modbus Safety Controller

IEC 61131-3 Structured Text motor state machine with fail-safe interlocks and a Modbus register contract.

## Validate

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The C++ reference model tests the Structured Text transition contract. Use a PLC vendor toolchain and hardware validation before deployment.
