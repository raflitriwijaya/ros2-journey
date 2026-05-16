# Phase 2 — Lesson 9: Custom Service

## Concept
Defining a custom `.srv` file and using it in a server/client pair.
Replaces the generic `std_srvs/SetBool` from Phase 1 with a
domain-specific irrigation control service.

## Service Definition — IrrigationControl.srv
REQUEST
- field_name          (string)
- duration_seconds    (float64)
- moisture_threshold  (float64)
---
RESPONSE
- success             (bool)
- message             (string)
- water_applied_liters (float64)

## Key C++ Concepts
- `using` alias avoids repeating long type names
- Service callback takes TWO placeholders: _1 (request), _2 (response)
- `async_send_request` returns a future (non-blocking)
- `spin_until_future_complete` blocks only until this response arrives

## Thesis Connection
This mirrors your STM32 pump command:
  Client = website sending irrigation command
  Server = STM32 executing the command and reporting water applied

## Run
Terminal 1: `ros2 run irrigation_robot irrigation_service_server`
Terminal 2: `ros2 run irrigation_robot irrigation_service_client`
