# Phase 2 — Lesson 10: Actions

## Concept
Actions handle long-running tasks with continuous feedback and a final result.
Third and final ROS2 communication primitive after topics and services.

## Action Definition — IrrigationCycle.action
GOAL
- field_name              (string)
- target_moisture_percent (float64)
- max_duration_seconds    (float64)
---
RESULT
- success                 (bool)
- final_moisture_percent  (float64)
- total_water_liters      (float64)
- message                 (string)
---
FEEDBACK
- current_moisture_percent (float64)
- water_applied_liters     (float64)
- elapsed_seconds          (float64)
- status_message           (string)

## Key C++ Concepts
- Server has 3 callbacks: handle_goal, handle_cancel, handle_accepted
- execute() runs in detached thread — server stays responsive
- publish_feedback() streams progress to client
- goal_handle->succeed() sends final result
- Client uses lambda callbacks for feedback and result separately

## All 3 Primitives Completed
| Type   | Pattern                        | Example              |
|--------|--------------------------------|----------------------|
| msg    | continuous stream, no reply    | SoilData every 1s    |
| srv    | instant request → response     | IrrigationControl    |
| action | long task + feedback + result  | IrrigationCycle      |

## Thesis Connection
Models a full irrigation cycle exactly as your STM32 runs it:
- Goal     = website sends target moisture + max runtime
- Feedback = STM32 reports moisture every second
- Result   = final moisture reached + total water consumed

## Run
Terminal 1: ros2 run irrigation_robot irrigation_action_server
Terminal 2: ros2 run irrigation_robot irrigation_action_client