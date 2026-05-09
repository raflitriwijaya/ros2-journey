# Phase 1 — ROS2 Fundamentals

**Level:** Beginner  
**Language:** C++ (rclcpp)  
**Package:** `irrigation_robot` (ament_cmake)

---

## 🎯 Milestone

> Build a complete pub/sub pipeline and a service server/client in C++ — all themed around the irrigation thesis domain.

---

## 📚 Lessons

| # | Topic | File | Status |
|---|-------|------|--------|
| [L1](./lesson1_nodes/) | Minimal Node | `minimal_node.cpp` | 🔄 In Progress |
| [L2](./lesson2_publisher/) | Publisher | `publisher_node.cpp` | ⏳ Upcoming |
| [L3](./lesson3_subscriber/) | Subscriber | `subscriber_node.cpp` | ⏳ Upcoming |
| [L4](./lesson4_services/) | Services | `service_server.cpp` / `service_client.cpp` | ⏳ Upcoming |
| [L5](./lesson5_parameters/) | Parameters | `param_node.cpp` | ⏳ Upcoming |
| [L6](./lesson6_launch_files/) | Launch Files | `irrigation.launch.py` | ⏳ Upcoming |

---

## 🧠 Core Mental Model

```
[soil_sensor_node] ──/soil_moisture──► [controller_node]
[controller_node]  ──/pump_command──►  [pump_driver_node]
[controller_node]  ────── /pump ◄────► [pump_controller]  ← service
```

Each box = one compiled C++ executable.  
Each arrow = a ROS2 topic or service — no direct function calls between nodes.

---

## 🔄 C++ Build Workflow

Every time you change code:

```bash
cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash
ros2 run irrigation_robot <executable_name>
```

> 💡 **Tip:** Add `--symlink-install` to colcon for faster iteration with headers.

---

## 🛠️ Commands Reference

```bash
# Node inspection
ros2 node list
ros2 node info /node_name

# Topic inspection
ros2 topic list
ros2 topic echo /topic_name
ros2 topic info /topic_name
ros2 topic hz /topic_name

# Service inspection
ros2 service list
ros2 service call /pump std_srvs/srv/SetBool "{data: true}"

# Build
colcon build --packages-select irrigation_robot
colcon build --packages-select irrigation_robot --cmake-args -DCMAKE_BUILD_TYPE=Release
```

---

## 📦 CMakeLists.txt Pattern

Every new `.cpp` file needs 3 lines in `CMakeLists.txt`:

```cmake
add_executable(my_node src/my_node.cpp)
ament_target_dependencies(my_node rclcpp std_msgs)
install(TARGETS my_node DESTINATION lib/${PROJECT_NAME})
```
