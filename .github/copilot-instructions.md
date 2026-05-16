# GitHub Copilot Instructions — ROS2 Journey

## Who I Am
- Agricultural Engineering graduate (Universitas Padjadjaran, 11 semesters)
- Thesis: Irrigation automation — STM32 (CAN Bus, LL/HAL), ESP32 (WiFiUDP),
  SIM900A, soil moisture sensors, crop water requirement control
- Goal: Master's degree at KTH Royal Institute of Technology
  — Systems, Control & Robotics (5-year plan)
- Parallel studies: Calculus, Linear Algebra, Probability & Statistics,
  Physics, STM32 low-layer, Grammar, Logic, Rhetoric

---

## This Repository
ROS2 learning journey from beginner to expert — 9 phases.
Every lesson is real, runnable C++ code inside a Docker container.

### Environment
- OS: Xubuntu 26
- ROS2: Jazzy Jalisco (Docker — `osrf/ros:jazzy-desktop`)
- Container: `ros2_session`
- Workspace: `/root/ros2_ws/` inside Docker
- Editor: VS Code with Dev Containers

### Packages
| Package | Type | Purpose |
|---------|------|---------|
| `irrigation_robot` | ament_cmake | All ROS2 lesson nodes |
| `irrigation_msgs` | ament_cmake | Custom msg/srv/action definitions |

---

## Strict Rules — Always Follow

### Language
- **C++ ONLY** — never suggest Python for ROS2 nodes
- Use `rclcpp`, not `rclpy`
- C++ standard: **C++17**

### File Naming
- Nodes: `lesson{N}_{description}.cpp` inside `irrigation_robot/src/`
- Never create subfolders inside `irrigation_robot/src/`
- Messages: `PascalCase.msg` inside `irrigation_msgs/msg/`
- Services: `PascalCase.srv` inside `irrigation_msgs/srv/`
- Actions: `PascalCase.action` inside `irrigation_msgs/action/`

### CMakeLists.txt Pattern (3 lines per executable)
```cmake
add_executable(node_name src/lesson{N}_{description}.cpp)
ament_target_dependencies(node_name rclcpp pkg_name)
install(TARGETS node_name DESTINATION lib/${PROJECT_NAME})
```

### Build Commands
```bash
colcon build --packages-select irrigation_robot
colcon build --packages-select irrigation_msgs
source install/setup.bash
```

### Git Commit Convention