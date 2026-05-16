# Phase 2 — Lesson 11: Executors and Callback Groups

## Concept
Executors control how ROS2 schedules and runs callbacks.
Callback groups control which callbacks can run in parallel.

## Executor Types
| Executor | Threads | Use case |
|----------|---------|----------|
| SingleThreadedExecutor | 1 | Simple nodes, default via spin() |
| MultiThreadedExecutor  | N | Parallel callbacks, real-time nodes |
| StaticSingleThreadedExecutor | 1 | Optimized, fixed node set |

## Callback Group Types
| Group | Behavior |
|-------|----------|
| Reentrant | Callbacks run in parallel |
| MutuallyExclusive | One callback at a time |

## Key C++ Concepts
- Assign groups via SubscriptionOptions for subscribers
- Pass group directly to create_service / create_timer
- MultiThreadedExecutor replaces rclcpp::spin()
- executor.add_node(node) then executor.spin()

## Thesis Connection
Equivalent to FreeRTOS task scheduling on STM32:
- Callback groups = task priorities
- MultiThreadedExecutor = RTOS scheduler
- Reentrant group = tasks that can preempt each other
- MutuallyExclusive group = tasks protected by mutex

## Run
Terminal 1: ros2 run irrigation_robot irrigation_executor
Terminal 2: ros2 run irrigation_robot soil_data_publisher
Terminal 3: ros2 service call /irrigation_control_ex ...