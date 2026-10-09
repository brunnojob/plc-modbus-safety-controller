# PLC Modbus Controller

Máquina de estados de motor com intertravamentos, referência Structured Text e processamento de quadros Modbus TCP.

## Executar

Requisitos: C++20 e CMake.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Funcionamento

Função 3 consulta registros; função 6 restringe escrita ao comando autorizado. Quadros MBAP inválidos e sequências repetidas são recusados. A lógica deve ser validada no PLC e nos intertravamentos físicos antes de comandar equipamentos.

## Persistência de resultados

O arquivo de operações está em [vercel-home-telemetry-api.vercel.app](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=plc-modbus-safety-controller). As migrações Supabase estão no [repositório da API](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue resultado.json --project plc-modbus-safety-controller
python cloud/sync.py sync
```

Defina `BRUNNODEV_ACCESS_TOKEN` com sua sessão. A fila SQLite conserva os relatórios até confirmação do servidor; o mesmo conteúdo não gera registros duplicados. Tokens não são gravados no código.
