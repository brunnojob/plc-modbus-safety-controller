# PLC Modbus Controller

A motor state machine with interlocks, a Structured Text reference, and Modbus TCP frame processing.

## Run

Requirements: C++20 and CMake.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Behavior

Function 3 reads registers; function 6 restricts writes to the authorized command. Invalid MBAP frames and repeated sequences are rejected. Validate the logic on the PLC and its physical interlocks before controlling equipment.

## Optional report archive

Use the [native C operations archive client](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/clients/c) to queue `result.json` under project `plc-modbus-safety-controller`. The client uses `BRUNNODEV_ACCESS_TOKEN` and retains unacknowledged reports locally.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).

## Implementation update

The C++ motor controller uses wrap-safe elapsed time across 32-bit clock rollover and rebases regressing clocks during transitions. Native tests verify rollover and stop timing. The Structured Text implementation remains available for PLC toolchains.

Contribution trailer: `Co-authored-by: nyctophile <33561761+ineedfoundmyway@users.noreply.github.com>`.
