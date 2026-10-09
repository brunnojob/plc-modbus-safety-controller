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

## Result synchronization

The [operations archive](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=plc-modbus-safety-controller) stores execution results. Supabase migrations are in the [API repository](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue result.json --project plc-modbus-safety-controller
python cloud/sync.py sync
```

Set `BRUNNODEV_ACCESS_TOKEN` to your session token. The SQLite outbox retains reports until the server confirms persistence; identical content does not create duplicate records. Tokens are not stored in source code. To run the synchronization tests:

```sh
python -m unittest discover -s cloud
```
