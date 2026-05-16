# Phase 2 — Lesson 8: Custom Message Subscriber

## Concept
Subscribing to a custom ROS2 message type (`irrigation_msgs/msg/SoilData`).
Completing the pub/sub pair started in L7.

## Key C++ Detail
Callback receives `const irrigation_msgs::msg::SoilData::SharedPtr msg`
Access fields with `->` operator: `msg->moisture_percent`, `msg->field_name`

## Run
Terminal 1: `ros2 run irrigation_robot soil_data_publisher`
Terminal 2: `ros2 run irrigation_robot soil_data_subscriber`

## Thesis Connection
Mirrors the ESP32 → STM32 data flow in your irrigation system.
Publisher = ESP32 sending soil moisture over WiFiUDP.
Subscriber = STM32 receiving and deciding pump action.
Now that logic lives in ROS2 nodes.
