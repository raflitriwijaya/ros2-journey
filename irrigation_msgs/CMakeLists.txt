cmake_minimum_required(VERSION 3.8)
project(irrigation_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/SoilData.msg"
  "srv/IrrigationControl.srv"
  "action/IrrigationCycle.action"
  DEPENDENCIES builtin_interfaces
)

ament_package()