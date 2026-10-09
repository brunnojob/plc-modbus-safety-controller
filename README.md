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

Export a JSON report from the command above, then run `python cloud/sync.py enqueue result.json --project plc-modbus-safety-controller` and `python cloud/sync.py sync`. Synchronization requires `BRUNNODEV_ACCESS_TOKEN` and the external operations API; the local outbox retains unacknowledged reports.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).
