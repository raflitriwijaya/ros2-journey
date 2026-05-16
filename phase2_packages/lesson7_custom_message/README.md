# Phase 2 — Lesson 7: Custom Message Publisher

## Concept
Publishing a custom ROS2 message type (`irrigation_msgs/msg/SoilData`)
instead of a primitive type like `std_msgs/String`.

## Files
- `irrigation_robot/src/lesson7_custom_msg_pub.cpp` — publisher node
- `irrigation_msgs/msg/SoilData.msg` — custom message definition

## Message Structure
| Field | Type | Description |
|-------|------|-------------|
| moisture_percent | float64 | Soil moisture reading (%) |
| temperature_celsius | float64 | Soil temperature |
| field_name | string | Identifier of the field |
| pump_active | bool | Whether pump should run |
| timestamp | builtin_interfaces/Time | When reading was taken |

## Run
```bash
ros2 run irrigation_robot lesson7_custom_msg_pub.cpp
ros2 topic echo /soil_data
```

## Key C++ Concepts
- Include path uses snake_case: `irrigation_msgs/msg/soil_data.hpp`
- `this->now()` returns `rclcpp::Time`, compatible with `builtin_interfaces/Time`
- `ament_target_dependencies` must list `irrigation_msgs` to resolve the header

## Thesis Connection
This mirrors the real SoilData packet your ESP32 sends over WiFiUDP —
same fields, now flowing through ROS2 topics instead of raw UDP.
