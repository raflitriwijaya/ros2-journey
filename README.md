# 🤖 ROS2 Journey — From Zero to KTH

> A documented C++ learning path from Agricultural Engineering  
> toward a Master's degree at **KTH Royal Institute of Technology**  
> in Systems, Control & Robotics.

---

## 👤 About Me

| | |
|---|---|
| **Background** | Agricultural Engineering — Universitas Padjadjaran (11 semesters) |
| **Thesis** | Irrigation automation — STM32 (CAN Bus, LL/HAL), ESP32 (WiFiUDP), SIM900A |
| **Goal** | Master's at KTH Royal Institute of Technology — Systems, Control & Robotics |
| **Language** | C++ (primary) — chosen for real-time performance, micro-ROS, and KTH research alignment |
| **Timeline** | 5 years self-study → KTH application |

---

## 🗺️ Roadmap Progress

| Phase | Topic | Level | Status |
|-------|-------|-------|--------|
| [P1](./phase1_fundamentals/) | ROS2 Fundamentals | Beginner | 🔄 In Progress |
| [P2](./phase2_packages/) | Packages & Workspace | Beginner+ | ⏳ Upcoming |
| [P3](./phase3_modeling/) | Robot Modeling & Visualization | Intermediate | ⏳ Upcoming |
| [P4](./phase4_perception/) | Perception & Sensors | Intermediate | ⏳ Upcoming |
| [P5](./phase5_nav2/) | Autonomous Navigation — Nav2 | Advanced | ⏳ Upcoming |
| [P6](./phase6_microros/) | Embedded & Hardware — micro-ROS | Advanced | ⏳ Upcoming |
| [P7](./phase7_moveit2/) | Manipulation — MoveIt2 | Advanced | ⏳ Upcoming |
| [P8](./phase8_autoware/) | Autonomous Vehicles — Autoware | Expert | ⏳ Upcoming |
| [P9](./phase9_research/) | Research & AI Integration | Expert | ⏳ Upcoming |

---

## ⚙️ Environment

| Tool | Detail |
|------|--------|
| OS | Xubuntu 26 |
| ROS2 | Jazzy Jalisco (via Docker) |
| Docker Image | `osrf/ros:jazzy-desktop` |
| Editor | VS Code + Dev Containers extension |
| Build System | colcon + CMake |
| Language | C++17 (rclcpp) |

### Quick Start

```bash
# Launch ROS2 container
~/ros2.sh

# Open extra terminal into running container
docker exec -it ros2_session bash

# Source ROS2 (every new terminal)
source /opt/ros/jazzy/setup.bash

# Build the package
cd /root/ros2_ws
colcon build --packages-select irrigation_robot

# Source the install after every build
source install/setup.bash
```

---

## 📁 Package Structure

```
ros2_ws/
└── irrigation_robot/          ← single ament_cmake package for all lessons
    ├── CMakeLists.txt
    ├── package.xml
    ├── include/irrigation_robot/
    └── src/
        ├── minimal_node.cpp        ← L1
        ├── publisher_node.cpp      ← L2
        ├── subscriber_node.cpp     ← L3
        ├── service_server.cpp      ← L4
        ├── service_client.cpp      ← L4
        ├── param_node.cpp          ← L5
        └── ...
```

---

## 📁 Repository Structure

```
ros2-journey/
├── README.md
├── phase1_fundamentals/
│   ├── lesson1_nodes/
│   ├── lesson2_publisher/
│   ├── lesson3_subscriber/
│   ├── lesson4_services/
│   ├── lesson5_parameters/
│   └── lesson6_launch_files/
├── phase2_packages/
├── phase3_modeling/
├── phase4_perception/
├── phase5_nav2/
├── phase6_microros/        ← STM32 + micro-ROS (your biggest advantage)
├── phase7_moveit2/
├── phase8_autoware/
└── phase9_research/
```

---

## 🔄 Why C++ over Python?

| Reason | Detail |
|--------|--------|
| **micro-ROS** | Only C/C++ supported on STM32 |
| **Real-time** | No GIL, deterministic timing |
| **KTH research** | Nav2, MoveIt2, Autoware all written in C++ |
| **STM32 background** | Already fluent in compiled, typed language |
| **Performance** | 10–100x faster for perception and control loops |

---

## 📝 Commit Convention

```
feat:     new lesson or concept added
fix:      bug or compile error corrected
docs:     README or notes updated
refactor: code cleaned up or restructured
exp:      experiment or exploration
build:    CMakeLists.txt changes
```

Example: `feat: P1L4 — service server and client in C++`

---

*Each commit = one real learning session. Updated continuously.*
